#include <Windows.h>
#include <tchar.h>
#include <vector>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 현재 도형의 타입을 저장하는 변수
int currentShapeType = 0;

// 보노보노 표시 여부 및 눈 깜빡임 변수
bool showBonobono = false;
bool isCrossedEyes = false;
bool blink = false;

// 그리기 진행 여부 및 HDC(HDC: Device Context) 변수
bool isDrawing = false;
HDC globalHdc = NULL;

// 마우스 드래그 시작 및 끝 포인트
POINT dragStartPoint = { 0 };
POINT dragEndPoint = { 0 };

// 이전에 그렸던 사각형을 저장하는 변수
RECT previousRect = { 0 };

// 그리기 영역을 나타내는 변수
RECT drawingRect = { 0 };

// 사용자 정의 도형 구조체
struct Circle {
    POINT center; // 원의 중심 좌표
    int radius;   // 원의 반지름
};

struct CustomShape {
    RECT rect;      // 도형의 위치와 크기
    int shapeType;  // 도형의 타입 

    union {
        Circle circle; // 원 정보
        // 필요한 경우 추가적인 도형 정보를 정의할 수 있음
    };
};

// 그려진 도형을 저장하는 벡터
std::vector<CustomShape> shapes;

// 외부 상자와 내부 상자를 그리는 함수
void DrawOuterAndInnerBoxes(HWND hWnd, HDC hdc) {
    // 클라이언트 영역의 사각형을 가져오기
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
    FillRect(hdc, &drawingRect, drawingBrush); // 변경된 부분
    FrameRect(hdc, &drawingRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

// 사각형을 그리는 함수
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

    RECT boxRect;
    boxRect.left = 20;
    boxRect.top = 100;
    boxRect.right = 760;
    boxRect.bottom = 425;

    // 여기에 사각형을 그리는 로직이 들어가야 합니다.
}

// 원을 그리는 함수
void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 브러시 생성
    SelectObject(hdc, hBrush);

    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

    DeleteObject(hBrush); // 브러시 삭제
}
// 보노보노를 그리는 함수
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
// 라이언을 그리는 함수
void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    int faceWidth = right - left;
    int faceHeight = bottom - top;

    // 눈 그리기
    int earsRadius = faceWidth / 8; // 동그란 눈의 반지름 계산

    // 브러시 생성 (검은색)
    HBRUSH hearsBrush = CreateSolidBrush(RGB(255, 200, 10));
    SelectObject(hdc, hearsBrush);

    // 왼쪽 눈 중심 좌표 및 그리기
    int earsEyeX = left + faceWidth / 3.7; // 왼쪽 눈의 중심 X 좌표
    int earsEyeY = top + faceHeight / 15; // 왼쪽 눈의 중심 Y 좌표
    Ellipse(hdc, earsEyeX - earsRadius, earsEyeY - earsRadius, earsEyeX + earsRadius, earsEyeY + earsRadius);

    // 오른쪽 눈 중심 좌표 및 그리기
    int rightearsX = right - faceWidth / 3.7; // 오른쪽 눈의 중심 X 좌표
    int rightearsY = top + faceHeight / 15; // 오른쪽 눈의 중심 Y 좌표
    Ellipse(hdc, rightearsX - earsRadius, rightearsY - earsRadius, rightearsX + earsRadius, rightearsY + earsRadius);

    // 브러시 삭제
    DeleteObject(hearsBrush);

    HBRUSH hFaceBrush = CreateSolidBrush(RGB(255, 200, 10)); // 노란색 브러시
    SelectObject(hdc, hFaceBrush);

    Ellipse(hdc, left, top, right, bottom); // 얼굴 그리기

    // 눈 그리기
    int eyeRadius = faceWidth / 20; // 동그란 눈의 반지름 계산

    // 브러시 생성 (검은색)
    HBRUSH hEyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hEyeBrush);

    // 왼쪽 눈 중심 좌표 및 그리기
    int leftEyeX = left + faceWidth / 3.7; // 왼쪽 눈의 중심 X 좌표
    int leftEyeY = top + faceHeight / 2.5; // 왼쪽 눈의 중심 Y 좌표
    Ellipse(hdc, leftEyeX - eyeRadius, leftEyeY - eyeRadius, leftEyeX + eyeRadius, leftEyeY + eyeRadius);

    // 오른쪽 눈 중심 좌표 및 그리기
    int rightEyeX = right - faceWidth / 3.7; // 오른쪽 눈의 중심 X 좌표
    int rightEyeY = top + faceHeight / 2.5; // 오른쪽 눈의 중심 Y 좌표
    Ellipse(hdc, rightEyeX - eyeRadius, rightEyeY - eyeRadius, rightEyeX + eyeRadius, rightEyeY + eyeRadius);

    // 브러시 삭제
    DeleteObject(hEyeBrush);

    // 코 그리기
    int bNoseRadius = faceWidth / 14; // 동그란 코의 반지름 계산

    HBRUSH hbNoseRadius = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hbNoseRadius);

    int leftbNoseX = left + faceWidth / 2.27; // 왼쪽 코의 중심 X 좌표
    int leftbNoseY = top + faceHeight / 1.6; // 왼쪽 코의 중심 Y 좌표
    Ellipse(hdc, leftbNoseX - bNoseRadius, leftbNoseY - bNoseRadius, leftbNoseX + bNoseRadius, leftbNoseY + bNoseRadius);

    int rightbNoseX = right - faceWidth / 2.27; // 오른쪽 코의 중심 X 좌표
    int rightbNoseY = top + faceHeight / 1.6; // 오른쪽 코의 중심 Y 좌표
    Ellipse(hdc, rightbNoseX - bNoseRadius, rightbNoseY - bNoseRadius, rightbNoseX + bNoseRadius, rightbNoseY + bNoseRadius);

    // 브러시 삭제
    DeleteObject(hbNoseRadius);

    // 검코 그리기
    int wNoseRadius = faceWidth / 20; // 동그란 검코의 반지름 계산

    // 브러시 생성 (검은색)
    HBRUSH hwNoseRadius = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hwNoseRadius);

    int leftwNoseX = left + faceWidth / 2; // 왼쪽 검코의 중심 X 좌표
    int leftwNoseY = top + faceHeight / 1.85; // 왼쪽 검코의 중심 Y 좌표
    Ellipse(hdc, leftwNoseX - wNoseRadius, leftwNoseY - wNoseRadius, leftwNoseX + wNoseRadius, leftwNoseY + wNoseRadius);

    // 브러시 삭제
    DeleteObject(hwNoseRadius);

    // 검은 눈썹 그리기
    HBRUSH hEyebrowPen = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hEyebrowPen);

    // 왼쪽 눈썹
    int leftEyebrowX1 = left + faceWidth / 3.7 - eyeRadius * 2;
    int leftEyebrowX2 = leftEyebrowX1 + faceWidth / 8 * 1.5;
    int leftEyebrowY = top + faceHeight / 2.5 - eyeRadius / 2 * 3.2;
    int leftEyebrowY1 = top + faceHeight / 2.5 - eyeRadius / 2 * 3.7;
    int leftEyebrowY2 = top + faceHeight / 2.5 - eyeRadius / 2 * 4.2;
    MoveToEx(hdc, leftEyebrowX1, leftEyebrowY, NULL);
    LineTo(hdc, leftEyebrowX2, leftEyebrowY);
    MoveToEx(hdc, leftEyebrowX1, leftEyebrowY1, NULL);
    LineTo(hdc, leftEyebrowX2, leftEyebrowY1);
    MoveToEx(hdc, leftEyebrowX1, leftEyebrowY2, NULL);
    LineTo(hdc, leftEyebrowX2, leftEyebrowY2);

    // 오른쪽 눈썹
    int rightEyebrowX1 = right - faceWidth / 3.7 + eyeRadius * 2;
    int rightEyebrowX2 = rightEyebrowX1 - faceWidth / 8 * 1.5;
    int rightEyebrowY = top + faceHeight / 2.5 - eyeRadius / 2 * 3.2;
    int rightEyebrowY1 = top + faceHeight / 2.5 - eyeRadius / 2 * 3.7;
    int rightEyebrowY2 = top + faceHeight / 2.5 - eyeRadius / 2 * 4.2;
    MoveToEx(hdc, rightEyebrowX1, rightEyebrowY, NULL);
    LineTo(hdc, rightEyebrowX2, rightEyebrowY);
    MoveToEx(hdc, rightEyebrowX1, rightEyebrowY1, NULL);
    LineTo(hdc, rightEyebrowX2, rightEyebrowY1);
    MoveToEx(hdc, rightEyebrowX1, rightEyebrowY2, NULL);
    LineTo(hdc, rightEyebrowX2, rightEyebrowY2);


    DeleteObject(hEyebrowPen);

    // 브러시 삭제
    DeleteObject(hFaceBrush);
}
