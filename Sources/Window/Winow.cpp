/**
 * @file CWinow.cpp
 * @brief
 * @author Yu Kimura
 * @date 2019/09/28
 */

#include "Winow.h"

namespace Simple 
{
    // コンストラクタ
    Window::Window(RECT windowRect) :
        m_hwnd(nullptr),
        m_hInstance(nullptr),
        m_className(),
        m_windowSize(windowRect.w, windowRect.h),
        m_clientSize(0, 0)
    {}

    // デストラクタ
    Window::~Window() {
        UnregisterClass(m_className, m_hInstance);
    }

    // ウインドウプロシージャ
    LRESULT Window::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

        switch (message) {
        case WM_CREATE:
            // ファイルのドロップを許可
            DragAcceptFiles(hwnd, true);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_MOUSEWHEEL:
            break;
        case WM_KEYDOWN:
            switch (wParam) {
            case VK_ESCAPE:
                if (IDYES == MessageBox(NULL, "終了しますか？", "MessageBox", MB_YESNO)) {
                    DestroyWindow(hwnd);
                }
                break;
            }
            break;
        case WM_DROPFILES:
        {
            // DROP構造体へのハンドル
            HDROP hDrop;
            char ch[256] = {};
            UINT fileSize = 0;

            hDrop = (HDROP)wParam;

            // ドロップされたファイル数を検索
            fileSize = DragQueryFile(hDrop, -1, NULL, 0);

            for (auto i = 0; i < (int)fileSize; ++i) {

                // ドロップされたファイルのファイルパスを取得
                DragQueryFile(hDrop, i, ch, sizeof(ch));

                //string str = ConvertString(ch);

                //CFileSystemManager::Get()->SetDrop(str);
            }

            //CFileSystemManager::Get()->SetLoadingFileType(FILE_TYPE::DROP_FILE);

            // ファイルの開放
            if (hDrop) DragFinish(hDrop);
        }
        break;
        default:

            break;
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    // ウインドウのサイズ変更
    void Window::ChangeSizeWindow(const int posx, const int posy, const int width, const int height) {

        SetWindowPos(
            m_hwnd,
            NULL,
            posx,
            posy,
            width,
            height,
            SWP_NOZORDER);

        ::RECT rClient;
        GetClientRect(m_hwnd, &rClient);

        m_windowSize.w = width;
        m_windowSize.h = height;

        m_clientSize.w = m_windowSize.w - (rClient.right - rClient.left);
        m_clientSize.h = m_windowSize.h - (rClient.bottom - rClient.top);
    }

    // ウインドウの初期化
    bool Window::Initialize(HINSTANCE hInstance, const char* className, const char* titleName) {

        WNDCLASSEX	wcex;
        wcex.hInstance = hInstance;                                 // インスタンス値のセット
        wcex.lpszClassName = className;                             // クラス名
        wcex.lpfnWndProc = WindowProc;                              // ウインドウメッセージ関数
        wcex.style = CS_HREDRAW | CS_VREDRAW;                       // ウインドウスタイル
        wcex.cbSize = sizeof(WNDCLASSEX);                           // 構造体のサイズ
        wcex.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);    // ラージアイコン
        wcex.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);      // スモールアイコン
        wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);      // カーソルスタイル
        wcex.lpszMenuName = NULL;                                   // メニューなし
        wcex.cbClsExtra = 0;                                        // エキストラなし
        wcex.cbWndExtra = 0;
        wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // 背景色白

        if (!RegisterClassEx(&wcex)) return false;                  // ウインドウクラスの登録

        m_hwnd = CreateWindow(
            className,                      // ウィンドウクラスの名前
            titleName,                      // タイトル
            WS_VISIBLE | WS_SYSMENU,        // ウィンドウスタイル
            0,                              // ウィンドウ位置_縦
            0,                              // ウィンドウ位置_横
            m_windowSize.w,                 // ウィンドウ横幅
            m_windowSize.h,                 // ウィンドウ縦幅
            NULL,                           // 親ウィンドウなし
            NULL,                           // メニューなし
            hInstance,                      // インスタンスハンドル
            NULL);                          // 追加引数なし

        if (!m_hwnd) return false;

        // ウインドウを表示する
        if (!ShowWindow(m_hwnd, SW_SHOW)) return false;
        if (!UpdateWindow(m_hwnd)) return false;

        m_hInstance = hInstance;
        m_className = className;

        ::RECT rClient;
        GetClientRect(m_hwnd, &rClient);

        m_clientSize.w = m_windowSize.w - (rClient.right - rClient.left);
        m_clientSize.h = m_windowSize.h - (rClient.bottom - rClient.top);

        return true;
    }

} // namespace