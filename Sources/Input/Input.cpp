/**
 * @file Input.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/11
 */

#include "Input/Input.h"

namespace Simple {

    // コンストラクタ
    Input::Input(HWND hwnd) :
        m_hWnd(hwnd),
        m_mouse(),
        m_key(),
        m_mousePos(),
        m_deltaMousePos(),
        m_oldKey(),
        m_oldMouse(),
        m_oldMousePos(),
        m_isKeyUpdate(true),
        m_isMouseUpdate(true)
    {}

    // デストラクタ
    Input::~Input() {
        if (m_pDirectDeviceKeyboard) 
            m_pDirectDeviceKeyboard->Unacquire();
        if (m_pDirectDeviceMouse) 
            m_pDirectDeviceMouse->Unacquire();
    }

    // 初期化
    bool Input::Initialize(HINSTANCE hInstance) {

        HRESULT hr = DirectInput8Create(
            hInstance,
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            (void**)&m_pDirectInput,
            nullptr);

        if (FAILED(hr)) {
            MessageBox(NULL, "DirectInput初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        if (!InitializeKeyBoard(m_hWnd)) return false;

        if (!InitializeMouse(m_hWnd)) return false;

        return true;
    }

    // キーボード初期化
    bool Input::InitializeKeyBoard(HWND hwnd) {

        HRESULT hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectDeviceKeyboard, nullptr);
        if (FAILED(hr)) {
            MessageBox(NULL, "キーボードデバイス初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        hr = m_pDirectDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(hr)) {
            MessageBox(NULL, "キーボードのデータフォーマット設定：失敗", "MessageBox", MB_OK);
            return false;
        }

        hr = m_pDirectDeviceKeyboard->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(hr)) {
            MessageBox(NULL, "キーボードの協調レベル設定：失敗", "MessageBox", MB_OK);
            return false;
        }

        // キーボードデバイスのアクセス権を取得
        m_pDirectDeviceKeyboard->Acquire();

        return true;
    }

    // マウス初期化
    bool Input::InitializeMouse(HWND hwnd) {

        HRESULT hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectDeviceMouse, nullptr);
        if (FAILED(hr)) {
            MessageBox(NULL, "マウスデバイス初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        hr = m_pDirectDeviceMouse->SetDataFormat(&c_dfDIMouse);
        if (FAILED(hr)) {
            MessageBox(NULL, "マウスのデータフォーマット設定：失敗", "MessageBox", MB_OK);
            return false;
        }

        hr = m_pDirectDeviceMouse->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(hr)) {
            MessageBox(NULL, "マウスの協調レベル設定：失敗", "MessageBox", MB_OK);
            return false;
        }

        // マウスデバイスのアクセス権を取得
        m_pDirectDeviceMouse->Acquire();

        return true;
    }

    // デバイス更新
    void Input::UpdateInput() {

        if (m_isMouseUpdate) {
            if (!UpdateMouse()) { 
            }
        }

        if (m_isKeyUpdate) {
            if (!UpdateKeyBoard()) {
            }
        }
    }

    // マウス更新ステートを変更
    void Input::ChangeMouseUpdateState(bool state) {
        m_isMouseUpdate = state;

        if (!m_isMouseUpdate) {
            m_mouse = {};
            m_oldMouse = {};
        }
    }

    // キーボード更新ステートを変更
    void Input::ChangeKeyUpdateState(bool state) {
        m_isKeyUpdate = state;

        if (!m_isKeyUpdate) {
            m_key = {};
            m_oldKey = {};
        }
    }

    // キーボード更新
    bool Input::UpdateKeyBoard() {

        std::copy(m_key.begin(), m_key.end(), m_oldKey.begin());

        HRESULT hr = m_pDirectDeviceKeyboard->GetDeviceState(sizeof(m_key), &m_key);
        if (FAILED(hr)) {
            m_pDirectDeviceKeyboard->Acquire();

            return false;
        }

        return true;
    }

    // マウス更新
    bool Input::UpdateMouse() {

        m_oldMouse = m_mouse;
        m_oldMousePos = m_mousePos;

        GetCursorPos(&m_mousePos);
        ScreenToClient(m_hWnd, &m_mousePos);

        m_deltaMousePos.x = m_oldMousePos.x - m_mousePos.x;
        m_deltaMousePos.y = m_oldMousePos.y - m_mousePos.y;

        HRESULT hr = m_pDirectDeviceMouse->GetDeviceState(sizeof(m_mouse), &m_mouse);
        if (FAILED(hr)) {
            m_pDirectDeviceMouse->Acquire();

            return false;
        }

        return true;
    }

    // キーが押されたかの取得
    bool Input::IsPressKey(UINT key) const 
    {
        return (m_key[key] & 0x80) != 0;
    }

    // キーが押され続けているかの取得
    bool Input::IsTriggerKey(UINT key) const 
    {
        return (m_key[key] & 0x80) && (m_oldKey[key] & 0x80);
    }

    // マウスの左が押されたかの取得
    bool Input::IsPressLeftMouse() const 
    {
        return (m_mouse.rgbButtons[0] & 0x80) != 0;
    }

    // マウスの左が押さ続けているかの取得
    bool Input::IsTriggerLeftMouse() const 
    {
        return (m_mouse.rgbButtons[0] & 0x80) && (m_oldMouse.rgbButtons[0] & 0x80);
    }

    // マウスの右が押されたかの取得
    bool Input::IsPressRightMouse() const 
    {
        return (m_mouse.rgbButtons[1] & 0x80) != 0;
    }

    // マウスの右が押さ続けているかの取得
    bool Input::IsTriggerRightMouse() const 
    {
        return (m_mouse.rgbButtons[1] & 0x80) && (m_oldMouse.rgbButtons[1] & 0x80);
    }

    // マウスのホイールが押されたかの取得
    bool Input::IsPressCenterMouse() const 
    {
        return (m_mouse.rgbButtons[2] & 0x80) != 0;
    }

    // マウスのホイールが押さ続けているかの取得
    bool Input::IsTriggerCenterMouse() const 
    {
        return (m_mouse.rgbButtons[2] & 0x80) && (m_mouse.rgbButtons[0] & 0x80);
    }

} // namespace