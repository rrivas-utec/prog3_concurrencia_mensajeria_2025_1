//
// Created by rrivas on 29/05/2025.
//

#ifndef QUEUE_H
#define QUEUE_H
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

namespace UTEC {
    template <typename T, template <class...> class Container = std::deque>
    class Queue {
        mutable std::shared_mutex mtx_;
        mutable std::condition_variable_any cv_;
        // std::atomic<bool> updated = false;
        Container<T> data_ = Container<T>();
    public:
        Queue() = default;
        template <typename Iterator>
        Queue(Iterator start, Iterator stop): data_(start, stop) {}
        Queue(std::initializer_list<T> lst):data_(lst) {}
        Queue(const Queue& other): data_(other.data_) {}
        Queue(Queue&& other) noexcept: data_(std::move(other.data_)) {}
        Queue& operator=(const Queue& other) {
            data_ = other.data_;
            return *this;
        }
        Queue& operator=(Queue&& other) noexcept {
            data_ = std::move(other.data_);
            return *this;
        }

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
            std::shared_lock lock(mtx_);
            return data_.front();
        }
        [[nodiscard]] T& back() {
            std::shared_lock lock(mtx_);
            return data_.back();
        }
        [[nodiscard]] const T& front() const {
            std::shared_lock lock(mtx_);
            return data_.front();
        }
        [[nodiscard]] const T& back() const {
            std::shared_lock lock(mtx_);
            return data_.back();
        }
        [[nodiscard]] size_t size() const {
            std::shared_lock lock(mtx_);
            return data_.size();
        }
        [[nodiscard]] bool empty() const {
            std::shared_lock lock(mtx_);
            return data_.empty();
        }
    };

}

#endif //QUEUE_H
