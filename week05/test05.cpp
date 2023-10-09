#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// 마우스 및 사각형 관련 변수들
POINT previousMousePos = { 0, 0 };
POINT startPoint = { 0 };
POINT endPoint = { 0 };
POINT newSPoint = { 0 };
POINT newEPoint = { 0 };
POINT movePoint = { 0 };
int isMouseLButtonPressed = 0;
int isMouseRButtonPressed = 0;

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        // 마우스 왼쪽 버튼 누를 때 시작점 저장
        startPoint.x = LOWORD(lParam);
        startPoint.y = HIWORD(lParam);
        isMouseLButtonPressed = 1;
    }
    break;

    case WM_LBUTTONUP:
    {
        // 마우스 왼쪽 버튼 놓을 때 끝점 저장 및 화면 갱신 요청
        endPoint.x = LOWORD(lParam);
        endPoint.y = HIWORD(lParam);
        isMouseLButtonPressed = 0;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        // 마우스 오른쪽 버튼 누를 때 시작점 저장
        newSPoint.x = LOWORD(lParam);
        newSPoint.y = HIWORD(lParam);
        isMouseRButtonPressed = 1;
    }
    break;

    case WM_RBUTTONUP:
    {
        // 마우스 오른쪽 버튼 놓을 때 끝점 저장 및 화면 갱신 요청
        newEPoint.x = LOWORD(lParam);
        newEPoint.y = HIWORD(lParam);
        isMouseRButtonPressed = 0;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        HDC hdc = GetDC(hwnd);

        // 마우스 왼쪽 버튼 누른 경우 사각형 그리기
        if (isMouseLButtonPressed)
        {
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
        }

        // 마우스 오른쪽 버튼 누르고 드래그한 경우 사각형 이동 및 그리기
        if (isMouseRButtonPressed && newSPoint.x > startPoint.x && newSPoint.y > startPoint.y && newEPoint.x < endPoint.x && newEPoint.y < endPoint.y)
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
            movePoint.x = newEPoint.x - newSPoint.x;
            movePoint.y = newEPoint.y - newSPoint.y;

            int left = min(startPoint.x + movePoint.x, endPoint.x + movePoint.x);
            int top = min(startPoint.y + movePoint.y, endPoint.y + movePoint.y);
            int right = max(startPoint.x + movePoint.x, endPoint.x + movePoint.x);
            int bottom = max(startPoint.y + movePoint.y, endPoint.y + movePoint.y);

            RECT Rect = { left, top, right, bottom };
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &Rect, hBrush);
            DeleteObject(hBrush);
        }

        ReleaseDC(hwnd, hdc);
    }
    break;

    case WM_DESTROY:
        // 애플리케이션 종료 메시지
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return S_OK;
}

#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    /* 윈도우 클래스 선언.*/
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));   // 모두 0으로 초기화.

    // 윈도우 클래스 값 설정
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software");
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;

    // 윈도우 클래스 등록.
    if (RegisterClass(&wc) == 0)
    {
        MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
        exit(-1);   //예외
    }

    // Window viewport 영역 조정
    RECT rect = { 150, 100, 800, 600 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // 윈도우 생성
    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("202207054 송유진"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        0, 0,
        width, height,
        NULL, NULL,
        hInstance,
        NULL
    );

    // 오류 검사.
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    // 창 보이기.
    ShowWindow(hwnd, SW_SHOW); // 창 띄우고
    UpdateWindow(hwnd);
