#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

ID2D1Factory* globalD2DFactory = nullptr;
ID2D1HwndRenderTarget* globalD2DRenderTarget = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        globalD2DRenderTarget->BeginDraw();
        globalD2DRenderTarget->Clear(
            D2D1::ColorF(D2D1::ColorF::CornflowerBlue)
        );
        globalD2DRenderTarget->EndDraw();
        ValidateRect(hWnd, nullptr);
        break;
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
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        return (0);
    }

    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory),
        nullptr,
        reinterpret_cast<void**>(&globalD2DFactory)
    );

    if (FAILED(hr))
    {
        CoUninitialize();
        return (0);
    }

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
        globalD2DFactory->Release();
        CoUninitialize();
        return (0);
    }

    RECT rc;
    GetClientRect(hWnd, &rc); // 창의 클라이언트 영역 크기.

    hr = globalD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
        ),
        &globalD2DRenderTarget
    );

    if (FAILED(hr))
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

    globalD2DRenderTarget->Release();
    globalD2DFactory->Release();
    CoUninitialize();
    return ((int)msg.wParam);
}
