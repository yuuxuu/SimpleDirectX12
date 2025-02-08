/**
 * @file App.cpp
 * @brief
 * @author Yu Kimura
 * @date 2018/12/15
 */

#include "App/App.h"

#include "Window/Winow.h"

#include "System/GameSystem.h"

#include "GraphicsAPI/DirectX12/DX12Graphics.h"

namespace Simple {

    // コンソールの呼び出し
    void CallConsole()
    {
        char Console[128];
        HWND ConsoleWindow;
        ::RECT ConsoleWindowRect;

        FILE* pfile = NULL; // ファイルポインタ

        // コンソール出力
        AllocConsole();
        // ストリームと結ぶ
        freopen_s(&pfile, "CONOUT$", "w", stdout);
        freopen_s(&pfile, "CONNIN$", "r", stdin);

        // コンソールタイトル検索
        GetConsoleTitle(Console, sizeof(Console));
        // コンソールウインドウハンドル取得
        ConsoleWindow = FindWindow(NULL, Console);
        // 現在のコンソールウインドウ位置を取得
        GetWindowRect(ConsoleWindow, &ConsoleWindowRect);
        // コンソールウインドウ位置変更
        MoveWindow(ConsoleWindow, 0, 0, ConsoleWindowRect.right - ConsoleWindowRect.left, ConsoleWindowRect.bottom - ConsoleWindowRect.top, TRUE);
    }
} // namespace

#if true
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
    Simple::CallConsole();

    Simple::Window window;
    if (!window.Initialize(hInstance, "SimpleDirectX12", "SimpleDirectX12"))
    {
        MessageBoxA(NULL, "ウィンドウ初期化：失敗", "MessageBox", MB_OK);
        return -1;
    }

    auto windowWidth = window.GetWindowSize().w;
    auto windowHeight = window.GetWindowSize().h;

    Graphics::DX12Graphics pGraphics;
    if (!pGraphics.Initialize(window.GetHwnd(), windowWidth, windowHeight))
    {
        MessageBoxA(NULL, "グラフィックデバイス初期化：失敗", "MessageBox", MB_OK);
        pGraphics.Finalize();
        return -1;
    }

    Simple::System::GameSystem gameSystem(&pGraphics);
    if (!gameSystem.Initialize(window.GetHwnd(), windowWidth, windowHeight))
    {
        MessageBoxA(NULL, "ゲームシステム初期化：失敗", "MessageBox", MB_OK);
        return -1;
    }

    MSG	msg;
    while (1) {
        if (!GetMessage(&msg, NULL, 0, 0)) {
            break;
        }
        else {
            // 文字メッセージへのコンバート
            TranslateMessage(&msg);
            // メッセージをWndProcへ送る
            DispatchMessage(&msg);
        }
    }

    gameSystem.Finalize();

    FreeConsole();

    return (int)msg.wParam;
}
#endif