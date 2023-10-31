#include <Windows.h>
#include <cmath>

#define IDC_BUTTON_BOX 1001
#define IDC_BUTTON_CIRCLE 1002

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool drawBox = false;
bool drawCircle = false;
POINT startDragPoint;
POINT endDragPoint;
bool isDragging = false;
RECT drawingRect;

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
        L"BasicWindow", L"곰돌이", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL
    );
    // Box 버튼 생성
    CreateWindow(
        L"BUTTON", L"Box", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 160, 64, hwnd, (HMENU)IDC_BUTTON_BOX, hInstance, NULL
    );

    // Circle 버튼 생성
    CreateWindow(
        L"BUTTON", L"Circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        180, 10, 160, 64, hwnd, (HMENU)IDC_BUTTON_CIRCLE, hInstance, NULL
    );
    // SetClassLongPtr를 사용하여 배경 브러시를 설정
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(CreateSolidBrush(RGB(255, 240, 200))));

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
        if (isDragging) {
            // 사각형 그리기 중에는 임시로 사각형을 그립니다.
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &drawingRect, hBrush);
            DeleteObject(hBrush);
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_COMMAND: {
        int buttonId = LOWORD(wParam);
        if (buttonId == IDC_BUTTON_BOX) {
            drawBox = true;
            drawCircle = false;
        }
        else if (buttonId == IDC_BUTTON_CIRCLE) {
            drawBox = false;
            drawCircle = true;
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        if (drawBox) {
            // Box 버튼을 누르고 좌클릭 드래그하면 검은색 사각형을 그리도록 플래그 설정
            isDragging = true;
            startDragPoint.x = LOWORD(lParam);
            startDragPoint.y = HIWORD(lParam);
            endDragPoint = startDragPoint; // 초기 위치를 시작 위치로 설정
        }
        else if (drawCircle) {
            // Circle 버튼을 누르고 좌클릭 드래그하면 원을 그립니다.
            startDragPoint.x = LOWORD(lParam);
            startDragPoint.y = HIWORD(lParam);
        }
        break;
    }

    case WM_MOUSEMOVE: {
        if (isDragging) {
            // 사각형 그리기 중에 마우스 이동 시
            endDragPoint.x = LOWORD(lParam);
            endDragPoint.y = HIWORD(lParam);
            drawingRect.left = min(startDragPoint.x, endDragPoint.x);
            drawingRect.top = min(startDragPoint.y, endDragPoint.y);
            drawingRect.right = max(startDragPoint.x, endDragPoint.x);
            drawingRect.bottom = max(startDragPoint.y, endDragPoint.y);
            InvalidateRect(hwnd, NULL, TRUE); // 화면 다시 그리기 요청
        }
        break;
    }

    case WM_LBUTTONUP: {
        if (drawBox && isDragging) {
            // 좌클릭 드래그를 끝내면 검은색 사각형을 그립니다.
            isDragging = false;
            HDC hdc = GetDC(hwnd);
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &drawingRect, hBrush);
            DeleteObject(hBrush);
            ReleaseDC(hwnd, hdc);
        }
        else if (drawCircle) {
            // Circle 버튼을 누르고 좌클릭 드래그를 끝내면 원을 그립니다.
            HDC hdc = GetDC(hwnd);
            int radius = (int)std::sqrt(std::pow(startDragPoint.x - LOWORD(lParam), 2) + std::pow(startDragPoint.y - HIWORD(lParam), 2));
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            Ellipse(hdc, startDragPoint.x - radius, startDragPoint.y - radius, startDragPoint.x + radius, startDragPoint.y + radius);
            DeleteObject(hBrush);
            ReleaseDC(hwnd, hdc);
        }
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