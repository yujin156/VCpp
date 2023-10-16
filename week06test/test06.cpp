#include <windows.h>

POINT startPoint = { 0 };
POINT endPoint = { 0 };
int isKeyPressed = 0;

RECT rect_user = { 5, 5, 10, 10 }; // ������� �簢��, ���� ��� ��ǥ (5, 5)���� ������ �ϴ� ��ǥ (10, 10)������ �簢��
RECT rect_target = { 50, 50, 150, 150 }; // Ÿ�� �簢��, ���� ��� ��ǥ (50, 50)���� ������ �ϴ� ��ǥ (150, 150)������ �簢��

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);

	HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255)); // ����� �簢���� �׸��� ��
	HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255)); // Ÿ�� �簢���� �׸��� ��
	HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // ���찳 ������ �ϴ� ��
	const wchar_t* text = L"Crash!!!"; // "Crash!!!" �ؽ�Ʈ�� ǥ���� �� ����ϴ� ���ڿ�

	switch (uMsg)
	{
	case WM_KEYDOWN: // Ű������ Ű�� ���� �� �߻��ϴ� �̺�Ʈ
		isKeyPressed = 1; // Ű�� �������� ǥ��
		if (wParam == VK_RIGHT) // ������ ȭ��ǥ Ű�� ������ ��
		{
			rect_user.left += 1; // ����� �簢���� ���������� �̵� (1 �ȼ�)
			rect_user.right += 1;
			InvalidateRect(hwnd, NULL, TRUE); // ȭ���� �����Ͽ� ��������� �ݿ�
		}
		else if (wParam == VK_LEFT) // ���� ȭ��ǥ Ű�� ������ ��
		{
			rect_user.left -= 1; // ����� �簢���� �������� �̵� (1 �ȼ�)
			rect_user.right -= 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_UP) // ���� ȭ��ǥ Ű�� ������ ��
		{
			rect_user.top -= 1; // ����� �簢���� ���� �̵� (1 �ȼ�)
			rect_user.bottom -= 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN) // �Ʒ��� ȭ��ǥ Ű�� ������ ��
		{
			rect_user.top += 1; // ����� �簢���� �Ʒ��� �̵� (1 �ȼ�)
			rect_user.bottom += 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_KEYUP: // Ű������ Ű�� ���� �� �߻��ϴ� �̺�Ʈ
		isKeyPressed = 0; // Ű�� �������� ǥ��
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		if (isKeyPressed) {
			if (rect_user.left < rect_target.right && rect_user.right > rect_target.left &&
				rect_user.top < rect_target.bottom && rect_user.bottom > rect_target.top) {
				TextOut(hdc, 10, 10, text, lstrlen(text)); // "Crash!!!" �޽����� ǥ��
				FillRect(hdc, &rect_target, hBrush_target); // ��ũ�� ������ �׸��ϴ�.
			}
			else {
				FillRect(hdc, &rect_target, hBrush_target); // ��ũ�� ������ �׸��ϴ�.
			}
		}
		else {
			FillRect(hdc, &rect_target, hBrush_target); // ��ũ�� ������ �׸��ϴ�.
		}

		// ó�� ������ �� �Ķ��� ������ �׸��ϴ�.
		FillRect(hdc, &rect_user, hBrush_user); // �Ķ��� ������ �׸��ϴ�.

		EndPaint(hwnd, &ps);
	}
	break;


	case WM_CLOSE: // �����츦 �������� �� ��
		DestroyWindow(hwnd); // �����츦 �ı�
		break;
	case WM_DESTROY: // �����찡 �ı��� ��
		PostQuitMessage(0); // ���� �޽����� ������ ���α׷� ����
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam); // �⺻ �̺�Ʈ ó�� ���� ����
	}

	DeleteObject(hBrush_user); // ����� �귯�� ����
	DeleteObject(hBrush_target); // Ÿ�� �귯�� ����
	DeleteObject(hBrush_eraser); // ���찳 �귯�� ����
	ReleaseDC(hwnd, hdc); // ����̽� ���ؽ�Ʈ ����

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
	ZeroMemory(&wc, sizeof(wc));	// ��� 0���� �ʱ�ȭ.

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software"); // ������ Ŭ���� �̸�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WindowProc;

	// ������ Ŭ���� ���.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//����
	}

	// Window viewport ���� ����
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ������ ����
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("202207054 ������"), // ������ Ÿ��Ʋ
		WS_OVERLAPPEDWINDOW,
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
	ShowWindow(hwnd, SW_SHOW); // �����츦 ȭ�鿡 ǥ��
	UpdateWindow(hwnd); // ȭ���� ������Ʈ�Ͽ� ���̰� ��

	// �޽��� ó�� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// �޽��� ó��.
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			// �޽��� �ؼ�����.
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ��������.
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance); // ������ Ŭ���� ��� ����

	// ���� �޽��� ������
	return (int)msg.wParam;
}
