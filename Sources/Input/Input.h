/**
 * @file Input.h
 * @brief
 * @author Yu Kimura
 * @date 2021/05/11
 */

#pragma once

#ifndef _Input_H_
#define _Input_H_

using Microsoft::WRL::ComPtr;

namespace Simple {

    constexpr int MAX_KEY = 256;

    class Input {
    private:
        HWND                            m_hWnd;

        ComPtr<IDirectInput8A>          m_pDirectInput;

        ComPtr<IDirectInputDevice8A>    m_pDirectDeviceKeyboard;
        ComPtr<IDirectInputDevice8A>    m_pDirectDeviceMouse;

        std::array<BYTE, MAX_KEY>       m_key;
        std::array<BYTE, MAX_KEY>       m_oldKey;

        DIMOUSESTATE                    m_mouse;
        DIMOUSESTATE                    m_oldMouse;

        POINT                           m_mousePos;
        POINT                           m_oldMousePos;
        POINT                           m_deltaMousePos;

        bool                            m_isKeyUpdate;
        bool                            m_isMouseUpdate;

    private:
        bool InitializeKeyBoard(HWND hwnd);
        bool InitializeMouse(HWND hwnd);

        bool UpdateKeyBoard();
        bool UpdateMouse();
    public:
        explicit Input(HWND hwnd);
        ~Input();

        bool Initialize(HINSTANCE hInstance);

        void UpdateInput();

        void ChangeMouseUpdateState(bool state);
        void ChangeKeyUpdateState(bool state);

        bool IsPressKey(UINT key) const;
        bool IsTriggerKey(UINT key) const;

        bool IsPressLeftMouse() const;
        bool IsTriggerLeftMouse() const;

        bool IsPressRightMouse() const;
        bool IsTriggerRightMouse() const;

        bool IsPressCenterMouse() const;
        bool IsTriggerCenterMouse() const;

        const float GetMousePosX() const { return (float)m_mouse.lX; }
        const float GetMousePosY() const { return (float)m_mouse.lY; }
        const float GetMousePosZ() const { return (float)m_mouse.lZ; }

        const float GetDeltaMousePosX() const { return (float)m_deltaMousePos.x; }
        const float GetDeltaMousePosY() const { return (float)m_deltaMousePos.y; }
    };

} // namespace

#endif // _Input_H_
