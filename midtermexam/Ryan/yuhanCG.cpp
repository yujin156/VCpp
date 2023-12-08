#include <Windows.h>
#include <tchar.h>
#include "yuhanCG.h"
#include <vector>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int currentShapeType = 0;

struct CustomShape {
    RECT rect;      // ������ ��ġ�� ũ��
    int shapeType;  // ������ Ÿ�� (0: �簢��)
};

int blink = 0; // ���� ������ �׸��� ����

bool isCrossedEyes = false;
bool spaceBarPressed = false;
bool isDrawing = false;
HDC globalHdc = NULL;

POINT dragStartPoint = { 0 };
POINT dragEndPoint = { 0 };

RECT previousRect = { 0 }; // ������ �׸� �簢���� ������ ����

std::vector<CustomShape> shapes; // �׸� ������ ������ ����



void DrawShapes(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush�� NULL�� �ʱ�ȭ

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������ �簢��
            SelectObject(hdc, hBrush);
            Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        /*else if (shapes[i].shapeType == 1) { // "Circle" ��� �׸��� ó��
            int centerX = 200; // ���� �߽� X ��ǥ
            int centerY = 200; // ���� �߽� Y ��ǥ
            int radius = 50;   // ���� ������

            // �� �׸���
            DrawCircle(hdc, centerX, centerY, radius);
        }*/
        else if (shapes[i].shapeType == 1) { // "Circle" ��� �׸��� ó��
            if (showBonobono) { // showBonobono �÷��װ� true�� ���� ���뺸�� ���� �׸����� ����
                DrawBonobono(hWnd, hdc, blink);
            }
        }

        else if (shapes[i].shapeType == 2) { // "Bonobono" ��� �׸��� ó��
            int centerX = 200; // ���� �߽� X ��ǥ
            int centerY = 200; // ���� �߽� Y ��ǥ
            int radius = 50;   // ���� ������

            // �� �׸���
            DrawCircle(hdc, centerX, centerY, radius);
        }

        else if (shapes[i].shapeType == 3) { // "��" ��� �׸��� ó��

            //DrawRyan(hWnd, hdc, drawingRect.left, drawingRect.top, drawingRect.right, drawingRect.bottom);
        }
        else if (shapes[i].shapeType == 4) { // "ť��" ��� �׸��� ó��
            if (showBonobono) { // showBonobono �÷��װ� true�� ���� ���뺸�� ���� �׸����� ����
                DrawBonobono(hWnd, hdc, blink);
            }
        }

        if (hBrush != NULL) {
            DeleteObject(hBrush);
            hBrush = NULL;
        }
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int buttonWidth = 142;
    int buttonHeight = 63;

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    switch (message) {
    case WM_CREATE: {
        int leftMargin = 20;
        int buttonSpacing = 8;

        CreateWindow(_T("BUTTON"), _T("Box"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftMargin, 20, buttonWidth, buttonHeight, hWnd, (HMENU)1, NULL, NULL);
        leftMargin += buttonWidth + buttonSpacing;

        CreateWindow(_T("BUTTON"), _T("Circle"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftMargin, 20, buttonWidth, buttonHeight, hWnd, (HMENU)3, NULL, NULL);
        leftMargin += buttonWidth + buttonSpacing;

        CreateWindow(_T("BUTTON"), _T("Bonobono"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftMargin, 20, buttonWidth, buttonHeight, hWnd, (HMENU)2, NULL, NULL);
        leftMargin += buttonWidth + buttonSpacing;

        CreateWindow(_T("BUTTON"), _T("Ryan"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftMargin, 20, buttonWidth, buttonHeight, hWnd, (HMENU)4, NULL, NULL);
        leftMargin += buttonWidth + buttonSpacing;

        CreateWindow(_T("BUTTON"), _T("Cube"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftMargin, 20, buttonWidth, buttonHeight, hWnd, (HMENU)5, NULL, NULL);
        leftMargin += buttonWidth + buttonSpacing;

        drawingRect.left = 20;
        drawingRect.top = 100;
        drawingRect.right = 760;
        drawingRect.bottom = 425;
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // "Box" ��ư�� ������ ��
            currentShapeType = 0;
            shapes.clear();
            isDrawing = true;
            SetFocus(hWnd);
            break;
        case 2: // "Circle" ��ư�� ������ ��
            currentShapeType = 1; // Circle ��ư�� ������ ���� �׸����� ����
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE); // Force immediate redraw
            SetFocus(hWnd);
            break;
        case 3: // "Bonobono" ��ư�� ������ ��
            currentShapeType = 2;
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE); // Force immediate redraw
            SetFocus(hWnd);
            break;
        case 4:
            
            currentShapeType = 3;
            // DrawRyan �Լ� ȣ��
            //DrawRyan(hWnd, globalHdc, drawingRect.left, drawingRect.top, drawingRect.right, drawingRect.bottom);

            SetFocus(hWnd);
            break;
        case 5:
            currentShapeType = 2;
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE); // â�� ��� �ٽ� �׸���
            break;;
        }
        break;


    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            spaceBarPressed = true;
            isCrossedEyes = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            spaceBarPressed = false;
            isCrossedEyes = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // �ܺ� ���ڿ� ���� ���ڸ� �׸��� �Լ� ȣ��
        DrawOuterAndInnerBoxes(hWnd, hdc);

        // ������ �׷ȴ� ������ DrawShapes�� ȣ������ �ʰ� ���� �� �ܺ� ���ڸ� �ٽ� �׸�
        if (!isDrawing) {
            DrawShapes(hWnd, hdc);
        }

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
        if (currentShapeType == 0 || currentShapeType == 1) {
            shapes.clear();
            isDrawing = true;
            dragStartPoint.x = LOWORD(lParam);
            dragStartPoint.y = HIWORD(lParam);
        }
        break;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON && isDrawing) {
            HDC hdc = GetDC(hWnd);
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            shapes.clear();
            CustomShape newShape;
            newShape.shapeType = currentShapeType;

            if (currentShapeType == 0) { // �簢�� �׸��� ����
                newShape.rect.left = min(dragStartPoint.x, x);
                newShape.rect.top = min(dragStartPoint.y, y);
                newShape.rect.right = max(dragStartPoint.x, x);
                newShape.rect.bottom = max(dragStartPoint.y, y);

                shapes.push_back(newShape);
                DrawShapes(hWnd, hdc);
            }
            else if (currentShapeType == 1) { // �� �׸��� ����
            // ���� ���콺 ��ġ�� ���������� ���� ������ ���
                int radius = static_cast<int>(sqrt(pow(x - dragStartPoint.x, 2) + pow(y - dragStartPoint.y, 2)));

                CustomShape newShape;
                newShape.shapeType = currentShapeType;
                newShape.rect.left = dragStartPoint.x - radius;
                newShape.rect.top = dragStartPoint.y - radius;
                newShape.rect.right = dragStartPoint.x + radius;
                newShape.rect.bottom = dragStartPoint.y + radius;

                shapes.push_back(newShape);
                DrawShapes(hWnd, hdc);
            }

            ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_LBUTTONUP:
        isDrawing = false;
        dragEndPoint.x = LOWORD(lParam);
        dragEndPoint.y = HIWORD(lParam);
        if (currentShapeType == 0) { // �簢�� �׸��� ���� �ϼ�
            CustomShape newShape;
            newShape.shapeType = currentShapeType;
            newShape.rect.left = min(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.top = min(dragStartPoint.y, dragEndPoint.y);
            newShape.rect.right = max(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.bottom = max(dragStartPoint.y, dragEndPoint.y);

            shapes.push_back(newShape);
            InvalidateRect(hWnd, NULL, TRUE);
        }

        if (currentShapeType == 1) { // �� �׸��� ���� �ϼ�
            int radius = static_cast<int>(sqrt(pow(dragEndPoint.x - dragStartPoint.x, 2) + pow(dragEndPoint.y - dragStartPoint.y, 2)));

            CustomShape newShape;
            newShape.shapeType = currentShapeType;
            newShape.rect.left = dragStartPoint.x - radius;
            newShape.rect.top = dragStartPoint.y - radius;
            newShape.rect.right = dragStartPoint.x + radius;
            newShape.rect.bottom = dragStartPoint.y + radius;

            shapes.push_back(newShape);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;


    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"BasicWindow";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
        L"BasicWindow", L"������", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
