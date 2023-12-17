#include "yuhanCG.h"

void DShape(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush를 NULL로 초기화

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 사각형
            SelectObject(hdc, hBrush);
            Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 1) { // "Bonobono" 모양 그리기 처리
            if (showBonobono) { // showBonobono 플래그가 true일 때만 보노보노 얼굴을 그리도록 수정
                DrawBonobono(hWnd, hdc, blink);
            }
        }

        else if (shapes[i].shapeType == 2) { // "Circle" 모양 그리기 처리
            DrawCircle(hdc, shapes[i].circle.center.x, shapes[i].circle.center.y, shapes[i].circle.radius);
        }

        else if (shapes[i].shapeType == 3) { // "Ryan" 모양 그리기 처리
            DrawRyan(hWnd, hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 4) { // "큐브" 모양 그리기 처리
            if (showBonobono) { // showBonobono 플래그가 true일 때만 보노보노 얼굴을 그리도록 수정
                DrawBonobono(hWnd, hdc, blink);
            }
        }
        // 브러시 객체를 삭제합니다.
        if (hBrush != NULL) {
            DeleteObject(hBrush);
            hBrush = NULL;
        }
    }
}

// 윈도우 프로시저 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int buttonWidth = 142; // 버튼의 너비
    int buttonHeight = 63; // 버튼의 높이

    RECT clientRect; // 클라이언트 영역의 사각형 정보를 담을 구조체
    GetClientRect(hWnd, &clientRect); // 윈도우의 클라이언트 영역 크기 정보를 가져옵니다.

    switch (message) {
    case WM_CREATE: { // 윈도우가 생성될 때
        int leftMargin = 20; // 버튼의 왼쪽 여백
        int buttonSpacing = 8; // 버튼 사이의 간격
// 다양한 버튼들을 생성합니다.
        // 각 버튼은 특정한 도형을 그리도록 설정됩니다.
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
        
        // 그리기 영역의 사각형을 초기화합니다.
        drawingRect.left = 20;
        drawingRect.top = 100;
        drawingRect.right = 760;
        drawingRect.bottom = 425;
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // "Box" 버튼을 눌렀을 때
            currentShapeType = 0;
            shapes.clear();
            isDrawing = true;
            showBonobono = false; // 보노보노 그림 지우기
            InvalidateRect(hWnd, NULL, TRUE); // 창을 즉시 다시 그리기
            SetFocus(hWnd);
            break;
        case 2: // "Bonobono" 버튼을 눌렀을 때
            currentShapeType = 1;
            shapes.clear(); // 저장된 모든 도형을 지움
            showBonobono = true;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case 3: // "Circle" 버튼을 눌렀을 때
            currentShapeType = 2;  // Circle 버튼을 누르면 원을 그리도록 설정
            shapes.clear();
            isDrawing = true;
            showBonobono = false; // 보노보노 그림 지우기
            InvalidateRect(hWnd, NULL, TRUE); // 창을 즉시 다시 그리기
            SetFocus(hWnd);
            break;
        case 4:

            currentShapeType = 3;
            // DrawRyan 함수 호출
            //DrawRyan(hWnd, globalHdc, drawingRect.left, drawingRect.top, drawingRect.right, drawingRect.bottom);
            shapes.clear(); // 저장된 모든 도형을 지움
            showBonobono = false; // 보노보노 그림 지우기
            InvalidateRect(hWnd, NULL, TRUE); // 창을 즉시 다시 그리기
            SetFocus(hWnd);
            break;
        case 5:
            currentShapeType = 2;
            shapes.clear(); // 저장된 모든 도형을 지움
            showBonobono = false; // 보노보노 그림 지우기
            InvalidateRect(hWnd, NULL, TRUE); // 창을 즉시 다시 그리기
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

        // 버튼을 누를 때, 키보드 입력에 따른 동작, 그리기 영역에서의 마우스 동작 등이 정의됩니다.
    case WM_PAINT: { // 화면을 다시 그려야 할 때
        PAINTSTRUCT ps; // 그리기 정보를 담을 구조체
        HDC hdc = BeginPaint(hWnd, &ps); // 그리기를 시작합니다.


        // 외부 상자와 내부 상자를 그리는 함수 호출
        DrawOuterAndInnerBoxes(hWnd, hdc); // 실제로는 해당 함수가 호출되어야 합니다.

        DShape(hWnd, hdc);
        if (showBonobono) {
            // 보노보노 그리는 로직이 여기에 작성되어야 합니다.
            DrawBonobono(hWnd, hdc, blink);
        }
        EndPaint(hWnd, &ps); // 그리기를 종료합니다.
        break;
    }

    case WM_LBUTTONDOWN:
        if (currentShapeType == 0) {
            shapes.clear();
            isDrawing = true;
            dragStartPoint.x = LOWORD(lParam);
            dragStartPoint.y = HIWORD(lParam);
        }
        if (currentShapeType == 2) { // Circle 버튼이 눌렸을 때
            shapes.clear();
            isDrawing = true;
            dragStartPoint.x = LOWORD(lParam);
            dragStartPoint.y = HIWORD(lParam);
        }
        if (currentShapeType == 3) { // Ryan 버튼이 눌린 경우
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

            if (currentShapeType == 0) { // 사각형 그리기 로직
                newShape.rect.left = min(dragStartPoint.x, x);
                newShape.rect.top = min(dragStartPoint.y, y);
                newShape.rect.right = max(dragStartPoint.x, x);
                newShape.rect.bottom = max(dragStartPoint.y, y);

                shapes.push_back(newShape);
                DShape(hWnd, hdc);
            }
            if (currentShapeType == 2) { // Circle 그리기 로직
                int radius = static_cast<int>(sqrt(pow(x - dragStartPoint.x, 2) + pow(y - dragStartPoint.y, 2)));

                newShape.circle.center.x = dragStartPoint.x;
                newShape.circle.center.y = dragStartPoint.y;
                newShape.circle.radius = radius;

                shapes.push_back(newShape);
                DShape(hWnd, hdc);
            }
            if (currentShapeType == 3 && isDrawing) { // Ryan 얼굴 그리기 로직
                // 이전에 그린 얼굴 지우기
                DrawOuterAndInnerBoxes(hWnd, hdc);
                DShape(hWnd, hdc);

                // 계산된 위치 및 크기로 새로운 Ryan 얼굴 정보 설정
                CustomShape newShape;
                newShape.shapeType = currentShapeType;
                newShape.rect.left = min(dragStartPoint.x, x);
                newShape.rect.top = min(dragStartPoint.y, y);
                newShape.rect.right = max(dragStartPoint.x, x);
                newShape.rect.bottom = max(dragStartPoint.y, y);

                // 드래그 중인 얼굴 실시간으로 그리기
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
        if (currentShapeType == 0) { // 사각형 그리기 로직 완성
            CustomShape newShape;
            newShape.shapeType = currentShapeType;
            newShape.rect.left = min(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.top = min(dragStartPoint.y, dragEndPoint.y);
            newShape.rect.right = max(dragStartPoint.x, dragEndPoint.x);
            newShape.rect.bottom = max(dragStartPoint.y, dragEndPoint.y);

            shapes.push_back(newShape);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (currentShapeType == 2) { // Circle 그리기 로직 완료
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
        if (currentShapeType == 3) { // Ryan 버튼이 눌린 경우
            isDrawing = false;
            dragEndPoint.x = LOWORD(lParam);
            dragEndPoint.y = HIWORD(lParam);

            // Ryan 얼굴 그리기 로직 추가
            CustomShape newShape;
            newShape.shapeType = currentShapeType;

            // 계산된 위치 및 크기로 Ryan 얼굴 정보 설정
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
// 프로그램의 메인 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) }; // 윈도우 클래스 구조체를 초기화합니다.
    // 필요한 윈도우 클래스 설정을 여기서 수행합니다.

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"BasicWindow";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    // 윈도우 클래스를 등록합니다.
    RegisterClassEx(&wc);
    // 윈도우를 생성합니다.
    HWND hwnd = CreateWindow(
        L"BasicWindow", L"곰돌이", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL, NULL, hInstance, NULL
    );
    // 윈도우를 보여줍니다.
    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    MSG msg; // 메시지 객체
    while (GetMessage(&msg, NULL, 0, 0)) { // 메시지 루프
        TranslateMessage(&msg); // 키 메시지를 처리합니다.
        DispatchMessage(&msg); // 윈도우 프로시저 함수에 메시지를 보냅니다.
    }

    return (int)msg.wParam; // 프로그램 종료
}
