//
// Created by rrivas on 29/05/2025.
//

#ifndef QUEUE_H
#define QUEUE_H
#include <deque>
#include <mutex>
#include <condition_variable>

namespace UTEC {
    template <typename T, template <class...> class Container = std::deque>
    class Queue {
        std::mutex mtx_;
        std::condition_variable cv_;
        // std::atomic<bool> updated = false;
        Container<T> data_ = Container<T>();
    public:
        Queue() = default;
        template <typename Iterator>
        Queue(Iterator start, Iterator stop): data_(start, stop) {}
        Queue(std::initializer_list<T> lst):data_(lst) {}
        Queue(const Queue& other) = delete;
        Queue(Queue&& other) = delete;
        Queue& operator=(const Queue& other) = delete;
        Queue& operator=(Queue&& other) = delete;

        void push(T item) {
            std::unique_lock lock(mtx_);
            data_.push_back(item);
            lock.unlock();
            // updated = true;
            cv_.notify_one();
        }
        template <typename ... Args>
        void emplace(Args ...args) {
            std::unique_lock lock(mtx_);
            data_.emplace_back(args...);
            lock.unlock();
            // updated = true;
            cv_.notify_one();
        }
        void pop() {
            // while (!updated) {}
            std::unique_lock lock(mtx_);
            cv_.wait(lock, [&]{ return !data_.empty(); });
            data_.pop_front();
            // updated = false;
        }
        [[nodiscard]] T& front() {
            std::lock_guard lock(mtx_);
            return data_.front();
        }
        [[nodiscard]] T& back() {
            std::lock_guard lock(mtx_);
            return data_.back();
        }
        [[nodiscard]] const T& front() const {
            // std::lock_guard lock(mtx_);
            return data_.front();
        }
        [[nodiscard]] const T& back() const {
            // std::lock_guard lock(mtx_);
            return data_.back();
        }
        [[nodiscard]] size_t size() {
            return data_.size();
        }
        [[nodiscard]] bool empty() {
            return data_.empty();
        }
    };

}

#endif //QUEUE_H
