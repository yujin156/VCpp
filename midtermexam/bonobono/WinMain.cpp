#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int currentShapeType = 0; // 도형 타입을 나타내는 변수
RECT drawingRect = { 0 }; // 드로잉 영역을 나타내는 RECT 구조체
bool showBonobono = false; // 보노보노 이미지를 표시할지 여부를 나타내는 변수
bool isCrossedEyes = false; // 눈이 교차한 상태 여부를 나타내는 변수
bool spaceBarPressed = false; // 스페이스바 누름 여부를 나타내는 변수

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"BasicWindow";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassEx(&wc);

    // 윈도우 생성 및 표시
    HWND hwnd = CreateWindow(
        L"BasicWindow", L"기본 윈도우", WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            currentShapeType = 0;
            showBonobono = false;
            SetFocus(hWnd);
            break;
        case 2:
            currentShapeType = 2;
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case 3:
            currentShapeType = 3;
            SetFocus(hWnd);
            break;
        case 4:
            currentShapeType = 1;
            SetFocus(hWnd);
            break;
        }
        break;
    case WM_CREATE: {
        CreateWindow(_T("BUTTON"), _T("Box"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 160, 64, hWnd, (HMENU)1, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("Circle"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 20, 160, 64, hWnd, (HMENU)3, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("Bonobono"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 380, 20, 160, 64, hWnd, (HMENU)2, NULL, NULL);
        CreateWindow(_T("BUTTON"), _T("Ryan"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 560, 20, 160, 64, hWnd, (HMENU)4, NULL, NULL);
        drawingRect.left = 20;
        drawingRect.top = 100;
        drawingRect.right = 760;
        drawingRect.bottom = 425;
        break;
    }
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            spaceBarPressed = true;
            isCrossedEyes = true; // 스페이스바를 누를 때 눈 교차
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            spaceBarPressed = false;
            isCrossedEyes = false; // 스페이스바를 뗄 때 눈 교차 해제
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        HBRUSH outsideBrush = CreateSolidBrush(RGB(255, 240, 200));
        FillRect(hdc, &clientRect, outsideBrush);

        RECT boxRect;
        boxRect.left = clientRect.left + 8;
        boxRect.top = clientRect.top + 8;
        boxRect.right = clientRect.right - 8;
        boxRect.bottom = clientRect.bottom - 8;

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200));
        FillRect(hdc, &boxRect, hBrush);

        FrameRect(hdc, &boxRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        HBRUSH drawingBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &drawingRect, drawingBrush);

        FrameRect(hdc, &drawingRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        if (showBonobono) {
            int bonobonoWidth = 100;
            int bonobonoHeight = 100;
            int bonobonoX = (drawingRect.left + drawingRect.right - bonobonoWidth) / 2;
            int bonobonoY = (drawingRect.top + drawingRect.bottom - bonobonoHeight) / 2;

            HBRUSH faceBrush = CreateSolidBrush(RGB(127, 200, 255));
            SelectObject(hdc, faceBrush);
            Ellipse(hdc, 200, 100, 400, 300);
            DeleteObject(faceBrush);


            if (spaceBarPressed) {
                HPEN eyeBrush = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                SelectObject(hdc, eyeBrush);

                int startX = bonobonoX;
                int startY = bonobonoY + 80;

                MoveToEx(hdc, startX - 105, startY - 115, NULL);
                LineTo(hdc, startX - 125, startY - 125);

                MoveToEx(hdc, startX - 105, startY - 114, NULL);
                LineTo(hdc, startX - 125, startY - 99);

                MoveToEx(hdc, startX + 20, startY - 115, NULL);
                LineTo(hdc, startX + 42, startY - 125);

                MoveToEx(hdc, startX + 21, startY - 114, NULL);
                LineTo(hdc, startX + 42, startY - 99);

                DeleteObject(eyeBrush);
            }
            else {
                HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc, eyeBrush);
                Ellipse(hdc, 220, 170, 230, 190); // 왼쪽 눈
                Ellipse(hdc, 370, 170, 380, 190); // 오른쪽 눈
                DeleteObject(eyeBrush);

                HBRUSH pupilBrush = CreateSolidBrush(RGB(255, 255, 255));
                SelectObject(hdc, pupilBrush);
                Ellipse(hdc, 222, 173, 227, 183); // 왼쪽 눈동자
                Ellipse(hdc, 372, 173, 377, 183); // 오른쪽 눈동자
                DeleteObject(pupilBrush);
            }




            HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 150, 255));
            SelectObject(hdc, mouthBrush);
            Ellipse(hdc, 287, 225, 313, 258);
            DeleteObject(mouthBrush);

            HBRUSH noseBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, noseBrush);
            Ellipse(hdc, 265, 239, 300, 204);
            Ellipse(hdc, 300, 239, 335, 204);
            DeleteObject(noseBrush);

            HBRUSH noseBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, noseBlackBrush);
            Ellipse(hdc, 286, 225, 314, 195);
            DeleteObject(noseBlackBrush);

            HPEN beardBrush = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            SelectObject(hdc, beardBrush);

            int startX = bonobonoX;
            int startY = bonobonoY + 80;

            MoveToEx(hdc, startX - 65, startY - 75, NULL);
            LineTo(hdc, startX - 95, startY - 85);

            MoveToEx(hdc, startX - 65, startY - 70, NULL);
            LineTo(hdc, startX - 95, startY - 65);

            MoveToEx(hdc, startX - 15, startY - 75, NULL);
            LineTo(hdc, startX + 15, startY - 85);

            MoveToEx(hdc, startX - 14, startY - 69, NULL);
            LineTo(hdc, startX + 15, startY - 59);

            DeleteObject(beardBrush);
        }

        EndPaint(hWnd, &ps);
        break;
    }
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
