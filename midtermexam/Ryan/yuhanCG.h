#include <Windows.h>
#include <tchar.h>

bool showBonobono = false;

RECT drawingRect = { 0 };

void DrawOuterAndInnerBoxes(HWND hWnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // 외부 상자 그리기
    HBRUSH outsideBrush = CreateSolidBrush(RGB(255, 240, 200));
    FillRect(hdc, &clientRect, outsideBrush);

    // 내부 상자 그리기
    RECT boxRect;
    boxRect.left = clientRect.left + 8;
    boxRect.top = clientRect.top + 8;
    boxRect.right = clientRect.right - 8;
    boxRect.bottom = clientRect.bottom - 8;

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200));
    FillRect(hdc, &boxRect, hBrush);
    FrameRect(hdc, &boxRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

    // 내부 상자의 빈 공간 채우기
    HBRUSH drawingBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &drawingRect, drawingBrush); // 이 부분이 변경된 부분입니다.
    FrameRect(hdc, &drawingRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}





void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom){
    // 라이언 얼굴 그리기
    HBRUSH faceBrush = CreateSolidBrush(RGB(255, 224, 189));
    SelectObject(hdc, faceBrush);
    Ellipse(hdc, left, top, right, bottom);
    DeleteObject(faceBrush);

    // 라이언 눈 그리기
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);
    Ellipse(hdc, left + 30, top + 30, left + 50, top + 50);  // 왼쪽 눈
    Ellipse(hdc, right - 50, top + 30, right - 30, top + 50);  // 오른쪽 눈
    DeleteObject(eyeBrush);

    // 라이언 코 그리기
    HPEN nosePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, nosePen);
    MoveToEx(hdc, (left + right) / 2, top + 50, NULL);
    LineTo(hdc, (left + right) / 2, bottom - 20);
    DeleteObject(nosePen);

    // 라이언 입 그리기
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, mouthBrush);
    Ellipse(hdc, left + 30, bottom - 30, right - 30, bottom - 10);
    DeleteObject(mouthBrush);

}

void DrawRectangle(HWND hwnd, POINT startPoint, POINT endPoint) {
    HDC hdc = GetDC(hwnd);

    RECT rect;
    GetClientRect(hwnd, &rect);
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

    int left = min(startPoint.x, endPoint.x);
    int top = min(startPoint.y, endPoint.y);
    int right = max(startPoint.x, endPoint.x);
    int bottom = max(startPoint.y, endPoint.y);

    RECT Rect = { left, top, right, bottom };
    HBRUSH hBrush = CreateSolidBrush(RGB(1, 13, 67));
    FillRect(hdc, &Rect, hBrush);
    DeleteObject(hBrush);

    ReleaseDC(hwnd, hdc);
}

void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
}

void DrawBonobono(HWND hWnd, HDC hdc, int blink) {
    int bonobonoWidth = 100;
    int bonobonoHeight = 100;
    int bonobonoX = (drawingRect.left + drawingRect.right - bonobonoWidth) / 2;
    int bonobonoY = (drawingRect.top + drawingRect.bottom - bonobonoHeight) / 2;

    HBRUSH faceBrush = CreateSolidBrush(RGB(127, 200, 255));
    SelectObject(hdc, faceBrush);
    Ellipse(hdc, 200, 100, 400, 300);
    DeleteObject(faceBrush);


    if (blink) {
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
