#include "MultiCore.hpp"
using namespace std;
namespace util {
    template <typename Task, typename TaskParamsDeAllocator, typename TaskResult> {
        void MultiCore::_run(){
            while(!_should_quite) {
                //pop a task from task q and execute it.
                unique_lock lck(_task_mutex);
                _task_available.wait(lck, [&_taskq] { return !_taskq.empty() || _should_quite; });
                if(!should_quite) {
                    TaskEntry &tsk = move(_taskq.front());
                    _taskq.popFront();
                    lck.unlock();
                    TaskResult result;
                    doTask(get<0>(tsk), get<1>(tsk), &result);
                    get<3>(tsk).set_value(move(result));
                    get<2>(tsk)(get<1>(tsk)); //deallocate the params.
                }
            }
        }
        virtual void doTask(const Task&, const void*, TaskResult*) = 0;
        future<TaskResult> enqueueTask(const Task& tsk, const void* params, TaskParamsDeAllocator deAllocator) {
            unique_lock lck(_task_mutex);
            promise<TaskResult> prms;
            auto ftr = prms.get_future();
            TaskEntry entry{tsk, params, deAllocator, move(prms)};
            _taskq.push_back(entry);
            _task_available.notify_one();
            return ftr;
        }
        bool MultiCore::start(){
            if(!_running.test_and_set()){
                //start two threads both running the _run function.
                try {
                    _threads = pair<thread, thread>(
                            thread(&MultiCore::_run, this),
                            thread(&MultiCore::_run, this),
                    );
                } catch (...) {
                    _threads = NULL;
                    return false;
                }
                return true;
            }
            return false;
        }
        std::future<> MultiCore::quite(){
            if(!_has_quit) {
                return async([this] {
                    _should_quite = true;
                    _task_available.notify_all();
                    _threads.first.join();
                    _threads.second.join();
                    _has_quit = true;
                });
            } else {
                return NULL;
            }
        }

        ~MultiCore::MultiCore(){
            if(!_has_quit) {
                quite().wait();
            }
            //deallocate all task params
            for(auto entry: _taskq) {
                get<2>(entry)(get<1>(entry)); //deallocate the params.
            }
        }
    }
}