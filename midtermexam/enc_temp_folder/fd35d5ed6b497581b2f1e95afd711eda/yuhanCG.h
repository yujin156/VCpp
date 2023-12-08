#include <Windows.h>
#include <tchar.h>


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

