#include <Windows.h>
#include <tchar.h>
#include <vector>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ���� ������ Ÿ���� �����ϴ� ����
int currentShapeType = 0;

// ���뺸�� ǥ�� ���� �� �� ������ ����
bool showBonobono = false;
bool isCrossedEyes = false;
bool blink = false;

// �׸��� ���� ���� �� HDC(HDC: Device Context) ����
bool isDrawing = false;
HDC globalHdc = NULL;

// ���콺 �巡�� ���� �� �� ����Ʈ
POINT dragStartPoint = { 0 };
POINT dragEndPoint = { 0 };

// ������ �׷ȴ� �簢���� �����ϴ� ����
RECT previousRect = { 0 };

// �׸��� ������ ��Ÿ���� ����
RECT drawingRect = { 0 };

// ����� ���� ���� ����ü
struct Circle {
    POINT center; // ���� �߽� ��ǥ
    int radius;   // ���� ������
};

struct CustomShape {
    RECT rect;      // ������ ��ġ�� ũ��
    int shapeType;  // ������ Ÿ�� 

    union {
        Circle circle; // �� ����
        // �ʿ��� ��� �߰����� ���� ������ ������ �� ����
    };
};

// �׷��� ������ �����ϴ� ����
std::vector<CustomShape> shapes;

// �ܺ� ���ڿ� ���� ���ڸ� �׸��� �Լ�
void DrawOuterAndInnerBoxes(HWND hWnd, HDC hdc) {
    // Ŭ���̾�Ʈ ������ �簢���� ��������
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // �ܺ� ���� �׸���
    HBRUSH outsideBrush = CreateSolidBrush(RGB(255, 240, 200));
    FillRect(hdc, &clientRect, outsideBrush);

    // ���� ���� �׸���
    RECT boxRect;
    boxRect.left = clientRect.left + 8;
    boxRect.top = clientRect.top + 8;
    boxRect.right = clientRect.right - 8;
    boxRect.bottom = clientRect.bottom - 8;

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200));
    FillRect(hdc, &boxRect, hBrush);
    FrameRect(hdc, &boxRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

    // ���� ������ �� ���� ä���
    HBRUSH drawingBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &drawingRect, drawingBrush); // ����� �κ�
    FrameRect(hdc, &drawingRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

// �簢���� �׸��� �Լ�
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

    // ���⿡ �簢���� �׸��� ������ ���� �մϴ�.
}

// ���� �׸��� �Լ�
void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ��� �귯�� ����
    SelectObject(hdc, hBrush);

    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

    DeleteObject(hBrush); // �귯�� ����
}
// ���뺸�븦 �׸��� �Լ�
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
        Ellipse(hdc, 220, 170, 230, 190); // ���� ��
        Ellipse(hdc, 370, 170, 380, 190); // ������ ��
        DeleteObject(eyeBrush);

        HBRUSH pupilBrush = CreateSolidBrush(RGB(255, 255, 255));
        SelectObject(hdc, pupilBrush);
        Ellipse(hdc, 222, 173, 227, 183); // ���� ������
        Ellipse(hdc, 372, 173, 377, 183); // ������ ������
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
// ���̾��� �׸��� �Լ�
void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    int faceWidth = right - left;
    int faceHeight = bottom - top;

    // �� �׸���
    int earsRadius = faceWidth / 8; // ���׶� ���� ������ ���

    // �귯�� ���� (������)
    HBRUSH hearsBrush = CreateSolidBrush(RGB(255, 200, 10));
    SelectObject(hdc, hearsBrush);

    // ���� �� �߽� ��ǥ �� �׸���
    int earsEyeX = left + faceWidth / 3.7; // ���� ���� �߽� X ��ǥ
    int earsEyeY = top + faceHeight / 15; // ���� ���� �߽� Y ��ǥ
    Ellipse(hdc, earsEyeX - earsRadius, earsEyeY - earsRadius, earsEyeX + earsRadius, earsEyeY + earsRadius);

    // ������ �� �߽� ��ǥ �� �׸���
    int rightearsX = right - faceWidth / 3.7; // ������ ���� �߽� X ��ǥ
    int rightearsY = top + faceHeight / 15; // ������ ���� �߽� Y ��ǥ
    Ellipse(hdc, rightearsX - earsRadius, rightearsY - earsRadius, rightearsX + earsRadius, rightearsY + earsRadius);

    // �귯�� ����
    DeleteObject(hearsBrush);

    HBRUSH hFaceBrush = CreateSolidBrush(RGB(255, 200, 10)); // ����� �귯��
    SelectObject(hdc, hFaceBrush);

    Ellipse(hdc, left, top, right, bottom); // �� �׸���

    // �� �׸���
    int eyeRadius = faceWidth / 20; // ���׶� ���� ������ ���

    // �귯�� ���� (������)
    HBRUSH hEyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hEyeBrush);

    // ���� �� �߽� ��ǥ �� �׸���
    int leftEyeX = left + faceWidth / 3.7; // ���� ���� �߽� X ��ǥ
    int leftEyeY = top + faceHeight / 2.5; // ���� ���� �߽� Y ��ǥ
    Ellipse(hdc, leftEyeX - eyeRadius, leftEyeY - eyeRadius, leftEyeX + eyeRadius, leftEyeY + eyeRadius);

    // ������ �� �߽� ��ǥ �� �׸���
    int rightEyeX = right - faceWidth / 3.7; // ������ ���� �߽� X ��ǥ
    int rightEyeY = top + faceHeight / 2.5; // ������ ���� �߽� Y ��ǥ
    Ellipse(hdc, rightEyeX - eyeRadius, rightEyeY - eyeRadius, rightEyeX + eyeRadius, rightEyeY + eyeRadius);

    // �귯�� ����
    DeleteObject(hEyeBrush);

    // �� �׸���
    int bNoseRadius = faceWidth / 14; // ���׶� ���� ������ ���

    HBRUSH hbNoseRadius = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hbNoseRadius);

    int leftbNoseX = left + faceWidth / 2.27; // ���� ���� �߽� X ��ǥ
    int leftbNoseY = top + faceHeight / 1.6; // ���� ���� �߽� Y ��ǥ
    Ellipse(hdc, leftbNoseX - bNoseRadius, leftbNoseY - bNoseRadius, leftbNoseX + bNoseRadius, leftbNoseY + bNoseRadius);

    int rightbNoseX = right - faceWidth / 2.27; // ������ ���� �߽� X ��ǥ
    int rightbNoseY = top + faceHeight / 1.6; // ������ ���� �߽� Y ��ǥ
    Ellipse(hdc, rightbNoseX - bNoseRadius, rightbNoseY - bNoseRadius, rightbNoseX + bNoseRadius, rightbNoseY + bNoseRadius);

    // �귯�� ����
    DeleteObject(hbNoseRadius);

    // ���� �׸���
    int wNoseRadius = faceWidth / 20; // ���׶� ������ ������ ���

    // �귯�� ���� (������)
    HBRUSH hwNoseRadius = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hwNoseRadius);

    int leftwNoseX = left + faceWidth / 2; // ���� ������ �߽� X ��ǥ
    int leftwNoseY = top + faceHeight / 1.85; // ���� ������ �߽� Y ��ǥ
    Ellipse(hdc, leftwNoseX - wNoseRadius, leftwNoseY - wNoseRadius, leftwNoseX + wNoseRadius, leftwNoseY + wNoseRadius);

    // �귯�� ����
    DeleteObject(hwNoseRadius);

    // ���� ���� �׸���
    HBRUSH hEyebrowPen = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hEyebrowPen);

    // ���� ����
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

    // ������ ����
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

    // �귯�� ����
    DeleteObject(hFaceBrush);
}
