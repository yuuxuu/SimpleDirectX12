/**
 * @file GameSystem.h
 * @brief
 * @author Yu Kimura
 * @date 2018/12/19
 */

#include "GameSystem.h"

#include "Input/Input.h"

#include "Scene/Scene.h"

#include "Camera/Camera.h"

namespace Simple
{
namespace System
{
    // コンストラクタ
    GameSystem::GameSystem(Graphics::IGraphics* pGraphics) :
        m_hGame(),
        pGraphics(pGraphics)
    {}

    // デストラクタ
    GameSystem::~GameSystem() {
        Finalize();
    }

    // 初期化
    bool GameSystem::Initialize(HWND hwnd, UINT windowWidth, UINT windowHeight)
    {
        m_pInput = std::make_unique<Simple::Input>(hwnd);
        if (!m_pInput->Initialize(GetModuleHandle(NULL)))
        {
            MessageBoxA(NULL, "インプットデバイス初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        m_pScene = std::make_unique<Simple::Scene>();
        m_pScene->SetUpScene(pGraphics, windowWidth, windowHeight);

        m_pCamera = std::make_unique<Simple::Camera>(windowWidth, windowHeight);
        m_pCamera->InitializeGraphicsResource(pGraphics);

        // イベントハンドル生成
        m_hGame = CreateEvent(NULL, false, false, NULL);
        if (!m_hGame) {
            MessageBoxA(NULL, "CGamesSystem::CreateEventエラー", "MessageBox", MB_OK);
            return false;
        }
        // ゲームメインスレッド生成
        m_gameSystemThread = std::thread(&GameSystem::Loop, this);
        if (!m_gameSystemThread.joinable()) {
            std::cout << "ゲームメインスレッド生成:失敗" << std::endl;
            return false;
        }

        return true;
    }

    // 終了
    void GameSystem::Finalize() {

        if (m_hGame) {
            CloseHandle(m_hGame);
            m_hGame = 0;
        }

        // ゲームメインスレッドの終了待ち
        if (m_gameSystemThread.joinable()) {
            m_gameSystemThread.join();
        }

        // タイマの分解能力を元に戻す
        timeEndPeriod(1);
    }

    // ループ
    void GameSystem::Loop() {
        DWORD sts;
        DWORD ms = 1000 / 60; // 1000ms/60fps = 0.16ms

        while (1) {
            sts = WaitForSingleObject(m_hGame, ms);
            if (sts == WAIT_FAILED) break;

            Update();
        }
    }

    void GameSystem::Update()
    {
        m_pInput->UpdateInput();

        m_pCamera->Update(*m_pInput.get());

        pGraphics->Update();

        m_pCamera->SetGraphicsResource(pGraphics);
        m_pScene->UpdateScene(pGraphics);

        pGraphics->Finalize();
    }
} // namespace System
} // namespace Simple