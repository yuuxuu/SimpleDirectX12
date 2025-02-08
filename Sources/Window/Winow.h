/**
 * @file CWinowManager.h
 * @brief
 * @author Yu Kimura
 * @date 2019/09/28
 */

#pragma once

#ifndef _CWINOW_H_ 
#define _CWINOW_H_

#include "math/math.h"

namespace Simple {

    class Window {
    private:
        HWND        m_hwnd;
        HINSTANCE   m_hInstance;
        const char* m_className;

        RECT        m_windowSize;
        RECT        m_clientSize;

    private:
        static LRESULT CALLBACK	WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        void ChangeSizeWindow(const int posx, const int posy, const int width, const int height);

    public:
        Window(RECT windowRect);
        ~Window();

        bool Initialize(HINSTANCE hInstance, const char* className, const char* titleName);

        HWND GetHwnd() const { return m_hwnd; }

        RECT GetWindowSize() const { return m_windowSize; }

        RECT GetClientSize() const { return m_clientSize; }

        HINSTANCE GethInstance() const { return m_hInstance; }
    };

} // namespace

#endif
