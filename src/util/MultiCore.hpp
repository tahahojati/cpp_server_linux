#ifndef CPP_SERVER_LINUX_MULTICORE_HPP
#define CPP_SERVER_LINUX_MULTICORE_HPP
#include <utility>
#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <deqeue>
#include <tuple>
#include <atomic>
//for now limited to two coresonly.
namespace util {
    template <typename Task, typename TaskParams, typename TaskResult>
    class MultiCore {
    private:
        typedef std::tuple<Task, TaskParams, std::promise<TaskResult>> TaskEntry;
        std::atomic_flag _running;
        std::deque<TaskEntry> _taskq;
        volatile bool _should_quite = false, _has_quit;
        std::pair<std::thread, std::thread> _treads;
        std::mutex _task_mutex;
        std::condition_variable _task_available;
        void _run();
    protected:
        virtual void doTask(const Task&, const TaskParams&, TaskResult*) = 0;
    public:
        future<TaskResult> enqueueTask(const Task& tsk, const TaskParams& prms);
        bool start();
        std::future<> quite();
        MultiCore() = default;
        MultiCore(const DB&) = delete;
        MultiCore(DB&&) = delete;
        MultiCore&operator=(const DB&) = delete;
        MultiCore&operator=(DB&&) = delete;
        ~MultiCore();
    };
}



#endif //CPP_SERVER_LINUX_MULTICORE_HPP
