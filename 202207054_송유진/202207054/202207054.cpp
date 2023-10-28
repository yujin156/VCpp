#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 기본 윈도우 배경색을 사용하거나
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"BasicWindow";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    // 윈도우 클래스 등록
    RegisterClassEx(&wc);

    // 윈도우 생성 및 표시
    HWND hwnd = CreateWindow(
        L"BasicWindow", L"기본 윈도우", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL
    );

    // SetClassLongPtr를 사용하여 배경 브러시를 설정
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 240, 200)));

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // Margin 값을 설정합니다.
        int margin = 8;

        // 박스의 크기를 계산합니다.
        RECT boxRect;
        boxRect.left = clientRect.left + margin;
        boxRect.top = clientRect.top + margin;
        boxRect.right = clientRect.right - margin;
        boxRect.bottom = clientRect.bottom - margin;

        // 박스의 내부를 r:255, g:240, b:200로 채웁니다.
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200));
        FillRect(hdc, &boxRect, hBrush);

        // 박스의 테두리를 검정으로 그립니다.
        FrameRect(hdc, &boxRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        DeleteObject(hBrush);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}