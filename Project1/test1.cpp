#include <windows.h>
#include <vector>
#include <tchar.h>
#include <math.h>

struct CustomShape {
    RECT rect;
    int shapeType;
};

std::vector<CustomShape> shapes;

int currentShapeType = 0; // 0: 사각형, 1: 삼각형, 2: 원, 3: 별

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

void DrawShapes(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL;

        switch (shapes[i].shapeType) {
        case 0:
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 사각형
            break;
        case 1:
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 삼각형
            break;
        case 2:
            hBrush = CreateSolidBrush(RGB(0, 255, 0)); // 초록색 원
            break;
        case 3:
            hBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색 별
            break;
        }

        if (hBrush != NULL) {
            SelectObject(hdc, hBrush);

            RECT& rect = shapes[i].rect;
            int shapeType = shapes[i].shapeType;

            if (shapeType == 0) {
                Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 1) {
                POINT points[3];
                points[0].x = (rect.left + rect.right) / 2;
                points[0].y = rect.top;
                points[1].x = rect.left;
                points[1].y = rect.bottom;
                points[2].x = rect.right;
                points[2].y = rect.bottom;
                Polygon(hdc, points, 3);
            }
            else if (shapeType == 2) {
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
            }
            else if (shapeType == 3) {
                POINT points[10];
                for (int j = 0; j < 10; j++) {
                    int radius = (j % 2 == 0) ? min(rect.right - rect.left, rect.bottom - rect.top) / 2 : min(rect.right - rect.left, rect.bottom - rect.top) / 4;
                    double angle = 3.14159265 * j / 5.0 - 3.14159265 / 2.0;
                    points[j].x = rect.left + (rect.right - rect.left) / 2 + radius * cos(angle);
                    points[j].y = rect.top + (rect.bottom - rect.top) / 2 + radius * sin(angle);
                }
                Polygon(hdc, points, 10);
            }

            DeleteObject(hBrush);
        }
    }

    if (isDrawing) {
        HBRUSH hBrush = NULL;

        switch (currentShapeType) {
        case 0:
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 사각형
            break;
        case 1:
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 삼각형
            break;
        case 2:
            hBrush = CreateSolidBrush(RGB(0, 255, 0)); // 초록색 원
            break;
        case 3:
            hBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색 별
            break;
        }

        if (hBrush != NULL) {
            SelectObject(hdc, hBrush);

            if (currentShapeType == 0) {
                Rectangle(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 1) {
                POINT points[3];
                points[0].x = (currentShapeRect.left + currentShapeRect.right) / 2;
                points[0].y = currentShapeRect.top;
                points[1].x = currentShapeRect.left;
                points[1].y = currentShapeRect.bottom;
                points[2].x = currentShapeRect.right;
                points[2].y = currentShapeRect.bottom;
                Polygon(hdc, points, 3);
            }
            else if (currentShapeType == 2) {
                Ellipse(hdc, currentShapeRect.left, currentShapeRect.top, currentShapeRect.right, currentShapeRect.bottom);
            }
            else if (currentShapeType == 3) {
                POINT points[10];
                for (int j = 0; j < 10; j++) {
                    int radius = (j % 2 == 0) ? min(currentShapeRect.right - currentShapeRect.left, currentShapeRect.bottom - currentShapeRect.top) / 2 : min(currentShapeRect.right - currentShapeRect.left, currentShapeRect.bottom - currentShapeRect.top) / 4;
                    double angle = 3.14159265 * j / 5.0 - 3.14159265 / 2.0;
                    points[j].x = currentShapeRect.left + (currentShapeRect.right - currentShapeRect.left) / 2 + radius * cos(angle);
                    points[j].y = currentShapeRect.top + (currentShapeRect.bottom - currentShapeRect.top) / 2 + radius * sin(angle);
                }
                Polygon(hdc, points, 10);
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
        case 1:
            currentShapeType = 0;
            break;
        case 2:
            currentShapeType = 1;
            break;
        case 3:
            currentShapeType = 2;
            break;
        case 4:
            currentShapeType = 3;
            break;
        }
        break;
    case WM_CREATE: {
        CreateWindow(_T("BUTTON"), _T("사각형"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 40, hWnd, (HMENU)1, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("삼각형"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 20, 100, 40, hWnd, (HMENU)2, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("원"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 240, 20, 100, 40, hWnd, (HMENU)3, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("별"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 20, 100, 40, hWnd, (HMENU)4, NULL, NULL);
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
