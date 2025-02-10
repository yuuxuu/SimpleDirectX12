/**
 * @file ThreadPoolSystem.cpp
 * @brief
 * @author 木村優
 * @date 2025/02/08
 */

#include "ThreadPoolSystem.h"

namespace Simple 
{
namespace System
{
    // コンストラクタ
    ThreadPoolSystem::ThreadPoolSystem() 
    {}

    // デストラクタ
    ThreadPoolSystem::~ThreadPoolSystem() 
    {
        Finalize();
    }
    
    // タスク処理
    void ThreadPoolSystem::WorkTask() {
        auto hr = CoInitialize(NULL);
        if (FAILED(hr))
            return;

        for (;;) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_conditionVariable.wait(lock, [&] { return !m_TaskQueue.empty() || !m_isRunning; });

                if (!m_isRunning && m_TaskQueue.empty()) {
                    break;
                }

                task = std::move(m_TaskQueue.front());
                m_TaskQueue.pop();
            }

            task();
        }
        CoUninitialize();
    }

    // タスクを追加
    void ThreadPoolSystem::AddTask(const std::function<void()>& task) 
    {
        const std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_isRunning) 
            throw std::runtime_error("Cannot schedule new task after shutdown");

        m_TaskQueue.push(task);

        m_conditionVariable.notify_one();
    }

    // 初期化
    void ThreadPoolSystem::Initialize(UINT numThread) 
    {
        for (UINT i = 0; i < numThread; ++i) 
            m_workThreadVec.push_back(std::make_unique<std::thread>(&ThreadPoolSystem::WorkTask, this));

        m_isRunning = true;
    }

    // 終了
    void ThreadPoolSystem::Finalize() 
    {
        if (!m_isRunning) return;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_isRunning = false;
        }

        m_conditionVariable.notify_all();

        int num = static_cast<int>(m_workThreadVec.size());
        int cntNum = 0;
        for (;;) 
        {
            for (const auto& thread : m_workThreadVec) 
            {
                if (thread.get()->joinable()) 
                {
                    thread.get()->join();
                    cntNum++;
                }
            }
            if (num == cntNum) break;
        }
    }
} // namespace System
} // namespace Simple