#include <windows.h>

POINT startPoint = { 0 };
POINT endPoint = { 0 };
int isKeyPressed = 0;

RECT rect_user = { 5, 5, 10, 10 }; // 사용자의 사각형, 왼쪽 상단 좌표 (5, 5)에서 오른쪽 하단 좌표 (10, 10)까지의 사각형
RECT rect_target = { 50, 50, 150, 150 }; // 타겟 사각형, 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);

	HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255)); // 사용자 사각형을 그리는 붓
	HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255)); // 타겟 사각형을 그리는 붓
	HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // 지우개 역할을 하는 붓
	const wchar_t* text = L"Crash!!!"; // "Crash!!!" 텍스트를 표시할 때 사용하는 문자열

	switch (uMsg)
	{
	case WM_KEYDOWN: // 키보드의 키를 누를 때 발생하는 이벤트
		isKeyPressed = 1; // 키가 눌렸음을 표시
		if (wParam == VK_RIGHT) // 오른쪽 화살표 키가 눌렸을 때
		{
			rect_user.left += 1; // 사용자 사각형을 오른쪽으로 이동 (1 픽셀)
			rect_user.right += 1;
			InvalidateRect(hwnd, NULL, TRUE); // 화면을 갱신하여 변경사항을 반영
		}
		else if (wParam == VK_LEFT) // 왼쪽 화살표 키가 눌렸을 때
		{
			rect_user.left -= 1; // 사용자 사각형을 왼쪽으로 이동 (1 픽셀)
			rect_user.right -= 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_UP) // 위쪽 화살표 키가 눌렸을 때
		{
			rect_user.top -= 1; // 사용자 사각형을 위로 이동 (1 픽셀)
			rect_user.bottom -= 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN) // 아래쪽 화살표 키가 눌렸을 때
		{
			rect_user.top += 1; // 사용자 사각형을 아래로 이동 (1 픽셀)
			rect_user.bottom += 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_KEYUP: // 키보드의 키를 놓을 때 발생하는 이벤트
		isKeyPressed = 0; // 키가 놓였음을 표시
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		if (isKeyPressed) {
			if (rect_user.left < rect_target.right && rect_user.right > rect_target.left &&
				rect_user.top < rect_target.bottom && rect_user.bottom > rect_target.top) {
				TextOut(hdc, 10, 10, text, lstrlen(text)); // "Crash!!!" 메시지를 표시
				FillRect(hdc, &rect_target, hBrush_target); // 핑크색 도형을 그립니다.
			}
			else {
				FillRect(hdc, &rect_target, hBrush_target); // 핑크색 도형을 그립니다.
			}
		}
		else {
			FillRect(hdc, &rect_target, hBrush_target); // 핑크색 도형을 그립니다.
		}

		// 처음 시작할 때 파랑색 도형도 그립니다.
		FillRect(hdc, &rect_user, hBrush_user); // 파랑색 도형을 그립니다.

		EndPaint(hwnd, &ps);
	}
	break;


	case WM_CLOSE: // 윈도우를 닫으려고 할 때
		DestroyWindow(hwnd); // 윈도우를 파괴
		break;
	case WM_DESTROY: // 윈도우가 파괴될 때
		PostQuitMessage(0); // 종료 메시지를 보내어 프로그램 종료
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam); // 기본 이벤트 처리 동작 수행
	}

	DeleteObject(hBrush_user); // 사용자 브러시 제거
	DeleteObject(hBrush_target); // 타겟 브러시 제거
	DeleteObject(hBrush_eraser); // 지우개 브러시 제거
	ReleaseDC(hwnd, hdc); // 디바이스 컨텍스트 해제

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
	ZeroMemory(&wc, sizeof(wc));	// 모두 0으로 초기화.

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software"); // 윈도우 클래스 이름
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WindowProc;

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("202207054 송유진"), // 윈도우 타이틀
		WS_OVERLAPPEDWINDOW,
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
	ShowWindow(hwnd, SW_SHOW); // 윈도우를 화면에 표시
	UpdateWindow(hwnd); // 화면을 업데이트하여 보이게 함

	// 메시지 처리 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// 메시지 처리.
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance); // 윈도우 클래스 등록 해제

	// 종료 메시지 보내기
	return (int)msg.wParam;
}
