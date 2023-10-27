#include <windows.h>
#include <vector>
#include <tchar.h>
#include <math.h>

struct CustomShape {
    RECT rect;
    int shapeType;
};

std::vector<CustomShape> shapes;

int currentShapeType = 0; // 0: 파란색 원, 1: 하얀색 원, 2: 주황색 원, 3: 검은색 원, 4: 검은색 사각형

bool isDrawing = false;
RECT currentShapeRect;
POINT dragStartPoint;
POINT dragEndPoint;

bool isMoving = false;
int movingShapeIndex = -1;
POINT moveOffset;
POINT mouseClickOffset; // 마우스 클릭 시 도형과 마우스 위치의 차이를 저장

HCURSOR hCursorArrow = LoadCursor(NULL, IDC_ARROW);
HCURSOR hCursorSizeAll = LoadCursor(NULL, IDC_SIZEALL);

int viewWidth = 800; // 뷰 가로 크기
int viewHeight = 480; // 뷰 세로 크기
COLORREF viewColor = RGB(255, 240, 200); // 뷰 배경 색상

void DrawShapes(HWND hWnd, HDC hdc) {
    // 뷰 배경 색상 설정
    HBRUSH hViewBrush = CreateSolidBrush(viewColor);
    SelectObject(hdc, hViewBrush);
    Rectangle(hdc, 0, 0, viewWidth, viewHeight);
    DeleteObject(hViewBrush);

    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL;

        switch (shapes[i].shapeType) {
        case 0:
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 원
            break;
        case 1:
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 하얀색 원
            break;
        case 2:
            hBrush = CreateSolidBrush(RGB(255, 165, 0)); // 주황색 원
            break;
        case 3:
            hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색 원
            break;
        case 4:
            hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색 사각형
            break;
        }

        if (hBrush != NULL) {
            SelectObject(hdc, hBrush);

            RECT& rect = shapes[i].rect;
            int shapeType = shapes[i].shapeType;

            if (shapeType == 0) {
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 1) {
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 2) {
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 3) {
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 4) {
                Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }

            DeleteObject(hBrush);
        }
    }

    if (isDrawing) {
        HBRUSH hBrush = NULL;

        switch (currentShapeType) {
        case 0:
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 원
            break;
        case 1:
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 하얀색 원
            break;
        case 2:
            hBrush = CreateSolidBrush(RGB(255, 165, 0)); // 주황색 원
            break;
        case 3:
            hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색 원
            break;
        case 4:
            hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색 사각형
            break;
        }

        if (hBrush != NULL) {
            SelectObject(hdc, hBrush);

            if (currentShapeType == 0) {
                Ellipse(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 1) {
                Ellipse(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 2) {
                Ellipse(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 3) {
                Ellipse(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 4) {
                Rectangle(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }

            DeleteObject(hBrush);
        }
    }
}

void StartDrawing(int x, int y) {
    isDrawing = true;
    currentShapeRect.left = x;
    currentShapeRect.top = y;
    currentShapeRect.right = x;
    currentShapeRect.bottom = y;
    dragStartPoint = { x, y };
    dragEndPoint = { x, y };
}

void UpdateDrawing(int x, int y) {
    if (isDrawing) {
        currentShapeRect.right = x;
        currentShapeRect.bottom = y;
        dragEndPoint = { x, y };
        InvalidateRect(NULL, NULL, TRUE);
    }
}

void StopDrawing() {
    if (isDrawing) {
        isDrawing = false;
        CustomShape newShape;
        newShape.rect = currentShapeRect;
        newShape.shapeType = currentShapeType;
        shapes.push_back(newShape);
        InvalidateRect(NULL, NULL, TRUE);
    }
}

void MoveShape(int x, int y) {
    if (isMoving && movingShapeIndex >= 0) {
        RECT& rect = shapes[movingShapeIndex].rect;
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        int centerX = x - moveOffset.x;
        int centerY = y - moveOffset.y;
        rect.left = centerX - width / 2;
        rect.top = centerY - height / 2;
        rect.right = centerX + width / 2;
        rect.bottom = centerY + height / 2;
        dragEndPoint.x = centerX;
        dragEndPoint.y = centerY;
        InvalidateRect(NULL, NULL, TRUE);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        StartDrawing(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
        StopDrawing();
        break;
    case WM_RBUTTONDOWN:
        for (size_t i = shapes.size(); i > 0; i--) {
            RECT& rect = shapes[i - 1].rect;
            POINT mousePos = { LOWORD(lParam), HIWORD(lParam) };

            // 도형 중심 계산
            POINT shapeCenter;
            shapeCenter.x = (rect.left + rect.right) / 2;
            shapeCenter.y = (rect.top + rect.bottom) / 2;

            // 도형 중심과 마우스 위치의 차이 계산
            mouseClickOffset.x = mousePos.x - shapeCenter.x;
            mouseClickOffset.y = mousePos.y - shapeCenter.y;

            if (PtInRect(&shapes[i - 1].rect, { LOWORD(lParam), HIWORD(lParam) }) && !isMoving) {
                isDrawing = false;
                isMoving = true;
                movingShapeIndex = i - 1;
                SetCursor(hCursorSizeAll);
                SetCapture(hWnd);
                break;
            }
        }
        break;
    case WM_RBUTTONUP:
        if (isMoving) {
            isMoving = false;
            ReleaseCapture();
            SetCursor(hCursorArrow);
        }
        break;
    case WM_MOUSEMOVE:
        if (isDrawing) {
            UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        if (isMoving) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (wParam & MK_RBUTTON) {
                MoveShape(x - mouseClickOffset.x, y - mouseClickOffset.y);
            }
        }
        break;
    case WM_SETCURSOR:
        if (isMoving || isDrawing) {
            SetCursor(hCursorArrow);
            return TRUE;
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawShapes(hWnd, hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 0:
            currentShapeType = 0;
            break;
        case 1:
            currentShapeType = 1;
            break;
        case 2:
            currentShapeType = 2;
            break;
        case 3:
            currentShapeType = 3;
            break;
        case 4:
            currentShapeType = 4;
            break;
        }
        break;
    case WM_CREATE: {
        CreateWindow(_T("BUTTON"), _T("파란색 원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 40, hWnd, (HMENU)0, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("하얀색 원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 20, 100, 40, hWnd, (HMENU)1, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("주황색 원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 240, 20, 100, 40, hWnd, (HMENU)2, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("검은색 원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 20, 100, 40, hWnd, (HMENU)3, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("검은색 사각형"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 460, 20, 150, 40, hWnd, (HMENU)4, NULL, NULL);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ButtonWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return 1;
    }

    hWnd = CreateWindow(
        L"ButtonWindowClass", L"Win32 Drawing Example", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
