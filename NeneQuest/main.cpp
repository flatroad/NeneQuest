#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    const WCHAR CLASS_NAME[] = L"NeneQuestWindowClass";
    const WCHAR TITLE[] = L"Nene Quest";
    WNDCLASSW WndClass = {};
    WndClass.lpfnWndProc = WndProc; // 이 창의 메시지를 처리할 함수.
    WndClass.hInstance = hInstance; // 이 창의 주인 (.exe).
    WndClass.lpszClassName = CLASS_NAME; // 클래스 이름표.
    WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW); // 기본 화살표 커서.

    RegisterClassW(&WndClass);

    HWND hWnd = CreateWindowExW(
        0, CLASS_NAME, TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720, nullptr, nullptr,
        hInstance, nullptr
    );

    if (hWnd == nullptr)
    {
        return (0);
    }

    ShowWindow(hWnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg); // 키 입력을 문자 메시지로 가공.
        DispatchMessage(&msg); // 메시지를 해당 창의 WndProc로 전달.
    }

    return ((int)msg.wParam);
}
