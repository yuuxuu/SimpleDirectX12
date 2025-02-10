/**
 * @file ThreadPoolSystem.h
 * @brief
 * @author 木村優
 * @date 2022/02/07
 */

#pragma once

#ifndef _THREAD_POOL_SYTEM_H_
#define _THREAD_POOL_SYTEM_H_

namespace Simple 
{
namespace System
{
    class ThreadPoolSystem 
    {
    private:
        std::vector<std::unique_ptr<std::thread>>   m_workThreadVec;

        std::queue<std::function<void()>>           m_TaskQueue;

        std::atomic<bool>                           m_isRunning;

        std::condition_variable                     m_conditionVariable;

        std::mutex                                  m_mutex;

    private:
        ThreadPoolSystem();
        ~ThreadPoolSystem();

        void WorkTask();

        void Finalize();

    public:
        static ThreadPoolSystem& GetThreadPoolSystem() 
        {
            static ThreadPoolSystem threadPoolSystem;
            return threadPoolSystem;
        }

        void Initialize(UINT numThread = std::thread::hardware_concurrency());

        void AddTask(const std::function<void()>& task);
    };
} // namespace System
} // namespace Simple

#endif // _THREAD_POOL_SYTEM_H_
