#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// ���콺 �� �簢�� ���� ������
POINT previousMousePos = { 0, 0 };
POINT startPoint = { 0 };
POINT endPoint = { 0 };
POINT newSPoint = { 0 };
POINT newEPoint = { 0 };
POINT movePoint = { 0 };
int isMouseLButtonPressed = 0;
int isMouseRButtonPressed = 0;

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        // ���콺 ���� ��ư ���� �� ������ ����
        startPoint.x = LOWORD(lParam);
        startPoint.y = HIWORD(lParam);
        isMouseLButtonPressed = 1;
    }
    break;

    case WM_LBUTTONUP:
    {
        // ���콺 ���� ��ư ���� �� ���� ���� �� ȭ�� ���� ��û
        endPoint.x = LOWORD(lParam);
        endPoint.y = HIWORD(lParam);
        isMouseLButtonPressed = 0;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        // ���콺 ������ ��ư ���� �� ������ ����
        newSPoint.x = LOWORD(lParam);
        newSPoint.y = HIWORD(lParam);
        isMouseRButtonPressed = 1;
    }
    break;

    case WM_RBUTTONUP:
    {
        // ���콺 ������ ��ư ���� �� ���� ���� �� ȭ�� ���� ��û
        newEPoint.x = LOWORD(lParam);
        newEPoint.y = HIWORD(lParam);
        isMouseRButtonPressed = 0;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        HDC hdc = GetDC(hwnd);

        // ���콺 ���� ��ư ���� ��� �簢�� �׸���
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

        // ���콺 ������ ��ư ������ �巡���� ��� �簢�� �̵� �� �׸���
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
        // ���ø����̼� ���� �޽���
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
    /* ������ Ŭ���� ����.*/
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));   // ��� 0���� �ʱ�ȭ.

    // ������ Ŭ���� �� ����
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software");
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;

    // ������ Ŭ���� ���.
    if (RegisterClass(&wc) == 0)
    {
        MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
        exit(-1);   //����
    }

    // Window viewport ���� ����
    RECT rect = { 150, 100, 800, 600 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // ������ ����
    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("202207054 ������"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        0, 0,
        width, height,
        NULL, NULL,
        hInstance,
        NULL
    );

    // ���� �˻�.
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    // â ���̱�.
    ShowWindow(hwnd, SW_SHOW); // â ����
    UpdateWindow(hwnd);
