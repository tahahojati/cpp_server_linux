#ifndef CPP_SERVER_LINUX_DATABASE_HPP
#define CPP_SERVER_LINUX_DATABASE_HPP
#include <unordered_map>
#include <string>
#include <future>
#include <condition_variable>
#include <mutex>
#include <deqeue>
#include <utility>
#include <atomic>

namespace database{
    class DB{
    private:
        enum class task {contains, set, get, del, end};
        std::atomic_flag _running;
        std::unordered_map<int, std::string> _db;
        std::deque<std::pair<task, void* params>> _taskq;
        std::mutex _writeMutex, _task_mutex;
        std::condition_variable _task_available;

        std::future<void>&& _quit();
        bool _popTaskAndDo();

    public:
        DB() = default;
        DB(const DB&) = delete;
        DB(DB&&) = delete;
        DB&operator=(const DB&) = delete;
        DB&operator=(DB&&) = delete;
        ~DB();
        std::future<bool>&& contains(int key) const;
        std::future<void>&& set (int key, const std::string& value);
        std::future<const std::string&>&& get (int key) const;
        std::future<std::string&&>&& del(int key);

        void run();
    } Database;
}

#endif //CPP_SERVER_LINUX_DATABASE_HPP
