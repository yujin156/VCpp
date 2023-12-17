#include "yuhanCG.h"

void DShape(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush�� NULL�� �ʱ�ȭ

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������ �簢��
            SelectObject(hdc, hBrush);
            Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 1) { // "Bonobono" ��� �׸��� ó��
            if (showBonobono) { // showBonobono �÷��װ� true�� ���� ���뺸�� ���� �׸����� ����
                DrawBonobono(hWnd, hdc, blink);
            }
        }

        else if (shapes[i].shapeType == 2) { // "Circle" ��� �׸��� ó��
            DrawCircle(hdc, shapes[i].circle.center.x, shapes[i].circle.center.y, shapes[i].circle.radius);
        }

        else if (shapes[i].shapeType == 3) { // "Ryan" ��� �׸��� ó��
            DrawRyan(hWnd, hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 4) { // "ť��" ��� �׸��� ó��
            if (showBonobono) { // showBonobono �÷��װ� true�� ���� ���뺸�� ���� �׸����� ����
                DrawBonobono(hWnd, hdc, blink);
            }
        }
        // �귯�� ��ü�� �����մϴ�.
        if (hBrush != NULL) {
            DeleteObject(hBrush);
            hBrush = NULL;
        }
    }
}

// ������ ���ν��� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int buttonWidth = 142; // ��ư�� �ʺ�
    int buttonHeight = 63; // ��ư�� ����

    RECT clientRect; // Ŭ���̾�Ʈ ������ �簢�� ������ ���� ����ü
    GetClientRect(hWnd, &clientRect); // �������� Ŭ���̾�Ʈ ���� ũ�� ������ �����ɴϴ�.

    switch (message) {
    case WM_CREATE: { // �����찡 ������ ��
        int leftMargin = 20; // ��ư�� ���� ����
        int buttonSpacing = 8; // ��ư ������ ����
// �پ��� ��ư���� �����մϴ�.
        // �� ��ư�� Ư���� ������ �׸����� �����˴ϴ�.
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
        
        // �׸��� ������ �簢���� �ʱ�ȭ�մϴ�.
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
            showBonobono = false; // ���뺸�� �׸� �����
            InvalidateRect(hWnd, NULL, TRUE); // â�� ��� �ٽ� �׸���
            SetFocus(hWnd);
            break;
        case 2: // "Bonobono" ��ư�� ������ ��
            currentShapeType = 1;
            shapes.clear(); // ����� ��� ������ ����
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case 3: // "Circle" ��ư�� ������ ��
            currentShapeType = 2;  // Circle ��ư�� ������ ���� �׸����� ����
            shapes.clear();
            isDrawing = true;
            showBonobono = false; // ���뺸�� �׸� �����
            InvalidateRect(hWnd, NULL, TRUE); // â�� ��� �ٽ� �׸���
            SetFocus(hWnd);
            break;
        case 4:

            currentShapeType = 3;
            // DrawRyan �Լ� ȣ��
            //DrawRyan(hWnd, globalHdc, drawingRect.left, drawingRect.top, drawingRect.right, drawingRect.bottom);
            shapes.clear(); // ����� ��� ������ ����
            showBonobono = false; // ���뺸�� �׸� �����
            InvalidateRect(hWnd, NULL, TRUE); // â�� ��� �ٽ� �׸���
            SetFocus(hWnd);
            break;
        case 5:
            currentShapeType = 2;
            shapes.clear(); // ����� ��� ������ ����
            showBonobono = false; // ���뺸�� �׸� �����
            InvalidateRect(hWnd, NULL, TRUE); // â�� ��� �ٽ� �׸���
            SetFocus(hWnd);
            break;;
        }
        break;


    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            blink = true;
            isCrossedEyes = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            blink = false;
            isCrossedEyes = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        // ��ư�� ���� ��, Ű���� �Է¿� ���� ����, �׸��� ���������� ���콺 ���� ���� ���ǵ˴ϴ�.
    case WM_PAINT: { // ȭ���� �ٽ� �׷��� �� ��
        PAINTSTRUCT ps; // �׸��� ������ ���� ����ü
        HDC hdc = BeginPaint(hWnd, &ps); // �׸��⸦ �����մϴ�.


        // �ܺ� ���ڿ� ���� ���ڸ� �׸��� �Լ� ȣ��
        DrawOuterAndInnerBoxes(hWnd, hdc); // �����δ� �ش� �Լ��� ȣ��Ǿ�� �մϴ�.

        DShape(hWnd, hdc);
        if (showBonobono) {
            // ���뺸�� �׸��� ������ ���⿡ �ۼ��Ǿ�� �մϴ�.
            DrawBonobono(hWnd, hdc, blink);
        }
        EndPaint(hWnd, &ps); // �׸��⸦ �����մϴ�.
        break;
    }

    case WM_LBUTTONDOWN:
        if (currentShapeType == 0) {
            shapes.clear();
            isDrawing = true;
            dragStartPoint.x = LOWORD(lParam);
            dragStartPoint.y = HIWORD(lParam);
        }
        if (currentShapeType == 2) { // Circle ��ư�� ������ ��
            shapes.clear();
            isDrawing = true;
            dragStartPoint.x = LOWORD(lParam);
            dragStartPoint.y = HIWORD(lParam);
        }
        if (currentShapeType == 3) { // Ryan ��ư�� ���� ���
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
                DShape(hWnd, hdc);
            }
            if (currentShapeType == 2) { // Circle �׸��� ����
                int radius = static_cast<int>(sqrt(pow(x - dragStartPoint.x, 2) + pow(y - dragStartPoint.y, 2)));

                newShape.circle.center.x = dragStartPoint.x;
                newShape.circle.center.y = dragStartPoint.y;
                newShape.circle.radius = radius;

                shapes.push_back(newShape);
                DShape(hWnd, hdc);
            }
            if (currentShapeType == 3 && isDrawing) { // Ryan �� �׸��� ����
                // ������ �׸� �� �����
                DrawOuterAndInnerBoxes(hWnd, hdc);
                DShape(hWnd, hdc);

                // ���� ��ġ �� ũ��� ���ο� Ryan �� ���� ����
                CustomShape newShape;
                newShape.shapeType = currentShapeType;
                newShape.rect.left = min(dragStartPoint.x, x);
                newShape.rect.top = min(dragStartPoint.y, y);
                newShape.rect.right = max(dragStartPoint.x, x);
                newShape.rect.bottom = max(dragStartPoint.y, y);

                // �巡�� ���� �� �ǽð����� �׸���
                shapes.push_back(newShape);
                DShape(hWnd, hdc);

                DrawRyan(hWnd, hdc, newShape.rect.left, newShape.rect.top, newShape.rect.right, newShape.rect.bottom);
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
        if (currentShapeType == 2) { // Circle �׸��� ���� �Ϸ�
            CustomShape newShape;
            newShape.shapeType = currentShapeType;

            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            int radius = static_cast<int>(sqrt(pow(x - dragStartPoint.x, 2) + pow(y - dragStartPoint.y, 2)));

            newShape.circle.center.x = dragStartPoint.x;
            newShape.circle.center.y = dragStartPoint.y;
            newShape.circle.radius = radius;

            shapes.push_back(newShape);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (currentShapeType == 3) { // Ryan ��ư�� ���� ���
            isDrawing = false;
            dragEndPoint.x = LOWORD(lParam);
            dragEndPoint.y = HIWORD(lParam);

            // Ryan �� �׸��� ���� �߰�
            CustomShape newShape;
            newShape.shapeType = currentShapeType;

            // ���� ��ġ �� ũ��� Ryan �� ���� ����
            newShape.rect.left = min(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.top = min(dragStartPoint.y, dragEndPoint.y);
            newShape.rect.right = max(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.bottom = max(dragStartPoint.y, dragEndPoint.y);

            shapes.push_back(newShape);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
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
// ���α׷��� ���� ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) }; // ������ Ŭ���� ����ü�� �ʱ�ȭ�մϴ�.
    // �ʿ��� ������ Ŭ���� ������ ���⼭ �����մϴ�.

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"BasicWindow";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    // ������ Ŭ������ ����մϴ�.
    RegisterClassEx(&wc);
    // �����츦 �����մϴ�.
    HWND hwnd = CreateWindow(
        L"BasicWindow", L"������", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL
    );
    // �����츦 �����ݴϴ�.
    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    MSG msg; // �޽��� ��ü
    while (GetMessage(&msg, NULL, 0, 0)) { // �޽��� ����
        TranslateMessage(&msg); // Ű �޽����� ó���մϴ�.
        DispatchMessage(&msg); // ������ ���ν��� �Լ��� �޽����� �����ϴ�.
    }

    return (int)msg.wParam; // ���α׷� ����
}
