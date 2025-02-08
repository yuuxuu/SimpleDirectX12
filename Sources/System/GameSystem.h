/**
 * @file GameSystem.h
 * @brief
 * @author Yu Kimura
 * @date 2018/12/19
 */

#pragma once

#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "GraphicsAPI/IGraphics.h"

namespace Simple 
{
    class Input;
    class Scene;
    class Camera;

namespace System
{
    class GameSystem 
    {
    private:
        std::thread                         m_gameSystemThread;
        HANDLE                              m_hGame;

        std::unique_ptr<Simple::Input>      m_pInput;
        std::unique_ptr<Simple::Scene>      m_pScene;
        std::unique_ptr<Simple::Camera>     m_pCamera;

        Graphics::IGraphics*                pGraphics;

    private:
        void Loop();
        void Update();

    public:
        explicit GameSystem(Graphics::IGraphics* pGraphics);
        ~GameSystem();

        bool Initialize(HWND hwnd, UINT windowWidth, UINT windowHeight);
        void Finalize();
    };
} // namespace System
} // namespace Simple

#endif // _GAME_SYSTEM_H_
