#include <windows.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "타이머";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}



int i = 0;
int totalSec;
SYSTEMTIME	st;
HWND btn_setting,btn_init;
HWND edit_h, edit_m, edit_s;

TCHAR time[50];
int hour, minute, second;

bool mode = false; // true면 타이머, false면 현재시간




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	PAINTSTRUCT ps;
	HDC hdc;
	static TCHAR sTime[128];
	static TCHAR timeUnit[10];
	HPEN  MyPen, OldPen;
	HBRUSH MyBrush, OldBrush;
	switch (message){

		// 타이머 리셋시키기
		// 타이머  
	case WM_CREATE:
		btn_setting = CreateWindow("button", "설정", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 350, 100, 25, hWnd, (HMENU)0x1001, g_hInst, NULL);
		btn_init = CreateWindow("button", "초기화", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 380, 100, 25, hWnd, (HMENU)0x1002, g_hInst, NULL);


		edit_h = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 100, 350, 50, 25, hWnd, (HMENU)0x1003, g_hInst, NULL);
		edit_m = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 180, 350, 50, 25, hWnd, (HMENU)0x1004, g_hInst, NULL);
		edit_s = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 260, 350, 50, 25, hWnd, (HMENU)0x1005, g_hInst, NULL);

		
		hour = 0, minute = 0, second = 0;
		SetTimer(hWnd, 1, 1000, NULL);
		
		return 0;

	case WM_TIMER:
		hdc = BeginPaint(hWnd, &ps);
		GetLocalTime(&st);

		if (mode == true){ //타이머 모드
			second--; //
			if (second % 60 == 0){
				minute--;
			}
			if (second % 3600 == 0){
				hour--;
			}
			wsprintf(sTime, "남은 시간 :  %d시 %d분 %d초", hour, minute, second);
		}
		else{   //현재시각 모드
			wsprintf(sTime, "현재 시각 :  %d시 %d분 %d초", st.wHour, st.wMinute, st.wSecond);
		}

		InvalidateRect(hWnd, NULL, true);
		return 0;

	case WM_COMMAND:
		//버튼클릭시, 숫자(time) 세팅 변경
		// 버튼 = 세팅버튼, 에디터박스: 시, 분, 초
		// 버튼 클릭시 초기화한거니 전역totalSec=0으로 초기화
		switch (LOWORD(wParam)){
		case 0x1001:
			mode = true; // 타이머 모드 발동
			GetWindowText(edit_h, time, 50);
			hour = (int)time;
			GetWindowText(edit_m, time, 50);
			minute = (int)time;
			GetWindowText(edit_s, time, 50);
			second = (int)time;

			hour = (int)time;
			//InvalidateRect(hWnd, NULL, true);
			break;

		case 0x1002:
			mode = false; // 현재시각, 초기화
			hour = 0;
			minute = 0;
			second = 0;
			//InvalidateRect(hWnd, NULL, true);
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		i = st.wSecond;


		MyBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		MoveToEx(hdc, 300, 300, NULL);
		Ellipse(hdc, 100, 100, 300, 300); //시계틀, 200X200크기

		MyBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Ellipse(hdc, 198, 198, 202, 202); //시계 중심의 위치

		SetTextAlign(hdc, TA_CENTER);

		wsprintf(timeUnit, TEXT("시"));
		TextOut(hdc, 160, 350, timeUnit, strlen(timeUnit));
		wsprintf(timeUnit, TEXT("분"));
		TextOut(hdc, 240, 350, timeUnit, strlen(timeUnit));
		wsprintf(timeUnit, TEXT("초"));
		TextOut(hdc, 320, 350, timeUnit, strlen(timeUnit));

		TextOut(hdc, 200, 320, sTime, strlen(sTime)); // 남은 시간 출력위치, 시간 초기화는 초마다.

		//시계 시각 표시
		for (i = 1; i <= 12; i++){
			wsprintf(sTime, "%d", i);
			TextOut(hdc, int(202 + 89 * cos((270 + 30 * i) * 3.14 / 180)),
				int(193 + 89 * sin((270 + 30 * i) * 3.14 / 180)), sTime, strlen(sTime));
		}

		if (mode == true){
			//반대로 돌아가게 만듦

			//초침
			MyPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 85 * cos((270 - 6 * second) * 3.14 / 180)),
				int(200 + 85 * sin((270 - 6 * second) * 3.14 / 180)));

			//분침
			MyPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 70 * cos((270 - 6 * minute + second / 10) * 3.14 / 180)),
				int(200 + 70 * sin((270 - 6 * minute + second / 10) * 3.14 / 180)));


			//시침
			MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 50 * cos((270 - 30 * hour + minute / 2) * 3.14 / 180)),
				int(200 + 50 * sin((270 - 30 * hour + minute / 2) * 3.14 / 180)));
		}
		else {
			// 아날로그 시계처럼 돌아가게 만들어야함

			//초침
			MyPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 85 * cos((270 + 6 * st.wSecond) * 3.14 / 180)),
				int(200 + 85 * sin((270 + 6 * st.wSecond) * 3.14 / 180)));

			//분침
			MyPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 70 * cos((270 + 6 * st.wMinute + st.wSecond / 10) * 3.14 / 180)),
				int(200 + 70 * sin((270 + 6 * st.wMinute + st.wSecond / 10) * 3.14 / 180)));

			//시침
			MyPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, int(200 + 50 * cos((270 + 30 * st.wHour + st.wMinute / 2) * 3.14 / 180)),
				int(200 + 50 * sin((270 + 30 * st.wHour + st.wMinute / 2) * 3.14 / 180)));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}