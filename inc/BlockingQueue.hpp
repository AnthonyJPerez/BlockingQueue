#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <iostream>

#include "Logger.h"


template<typename T>
class BlockingQueue
{
private:
    std::mutex mutex_;
    std::condition_variable conditionPush_;
    std::condition_variable conditionPop_;
    std::deque<T> queue_;
    bool isBlocking_ = true;

    std::uint16_t maxSize_ = 3;


    T pop_blockOnEmpty()
    {
        T val;
        LOG("");
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            LOG("obtained lock");
            this->conditionPop_.wait(lock, [=] {
                LOG("Checking wait condition");
                if (isBlocking_)
                {
                    LOG("isBlocking == true");
                }
                else
                {
                    LOG("isBlocking == false");
                }
                if (this->queue_.empty())
                {
                    LOG("queue.empty() == true");
                }
                else
                {
                    LOG("queue.empty() == false");
                }
                return !isBlocking_ || !this->queue_.empty();
            });

            if (!this->queue_.empty())
            {
                LOG("queue contains data, popping...");
                val = std::move(this->queue_.back());
                this->queue_.pop_back();
                LOG("data popped!");
                LOG(val);
            }
            else
            {
                LOG("queue empty, leaving...");
            }
        }
        this->conditionPush_.notify_one();
        LOG("return");
        return val;
    }

public:
    BlockingQueue()
            : mutex_()
            , conditionPush_()
            , conditionPop_()
            , queue_()
    {}


    void push(T const &value)
    {
        LOG(value);
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            LOG("obtained lock");

            this->conditionPush_.wait(lock, [=] { return !isBlocking_ || this->queue_.size() < maxSize_; });
            LOG("queue has room, pushing...");
            LOG(value);
            queue_.push_front(value);
        }
        this->conditionPop_.notify_one();
        LOG("return");
    }


    T pop(bool blockOnEmpty = true)
    {
        if (blockOnEmpty)
        {
            return pop_blockOnEmpty();
        }

        std::unique_lock<std::mutex> lock(this->mutex_);
        T val = std::move(this->queue_.back());
        this->conditionPush_.notify_one();
        return val;
    }


    void setBlocking(bool isBlocking)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        isBlocking_ = isBlocking;

        if (isBlocking)
        {
            LOG("Blocking set to true");
        }
        else
        {
            LOG("Blocking set to false");
            this->conditionPush_.notify_all();
            this->conditionPop_.notify_all();
        }
    }


    void setMaxSize(const std::uint32_t maxSize)
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->maxSize_ = maxSize;
    }


    bool isEmpty()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return this->queue_.empty();
    }
};

#endif //_BLOCKINGQUEUE_H
