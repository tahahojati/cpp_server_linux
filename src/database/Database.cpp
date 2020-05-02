#include "Database.hpp"
#include <thread>

namespace database{
    using namespace std;
    enum class task {contains, set, get, del};
    unordered_map<int, string> _db;
    deque<task, void* params> _taskq;
    mutex _writeMutex, _task_mutex; //Always first obtain task mutex then write mutex.
    condition_variable _task_available;

    DB::~DB(){}
    future<bool>&& DB::contains(int key) const{

    }
    future<void>&& DB::set (int key, const string& value){}
    future<const string&>&& DB::get (int key) const{}
    future<string&&>&& DB::del(int key){}

    void DB::run(){
        if(!_running.test_and_set()) {

        }
    }

    future<void>&& DB::_quit(){

    }

    bool DB::_popTaskAndDo() {
        unique_lock tsklck(_task_mutex);
        unique_lock datalck (_writeMutex, defer_lock);
        _task_available.wait(tsklck, [&_taskq]{
            return !_taskq.empty(); //If not empty continue. If empty, wait.
        });
        pair<task, void*> tsk(_taskq.front()); //copy construction. Because we don't want the pop_front operation to
        // delete the task.
        if(tsk.first != task::end) {
            _taskq.pop_front();
            tsklck.unlock();
            switch (tsk.first) {
                //do the actual work on the data
                case task::set:
                    datalck.lock();
                    auto entry_p = static_cast<pair<int, string>*>(tsk.second);
                    _db[entry_p -> first] = move(entry_p -> second);
                    datalck.unlock();
                    delete entry_p;
                    break;
                case task::del:
                    datalck.lock();
                    auto key_p = static_cast<int*>(tsk.second);
                    _db.erase(*key_p);
                    datalck.unlock();
                    delete key_p;
                    break;
                case task::contains:
                    datalck.lock();
                    auto key_p = static_cast<int*>(tsk.second);
                    _db.;
                    datalck.unlock();
                    delete key_p;
                    break;
                case task::get:
                    break;
            }
            return false;
        } else {
            return true; //return value specifies if the thread should terminate.  True means the thread should
            // terminate.
        }
    }

}