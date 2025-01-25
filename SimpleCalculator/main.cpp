#include <Windows.h>
#include <CommCtrl.h>
#include "Buttons.h"

#include <cstdint>

void initCalcButtons(HWND hwnd) {
	RECT rc{};
	GetClientRect(hwnd, &rc);

	const int buttonWidth{ rc.right / 3 };
	const int buttonHeight{ rc.bottom / 6 };

	int colAdj{ 1 };
	int rowAdj{};

	for (const ButtonDesc& bd : buttonsDescs) {
		CreateWindow(
			TEXT("Button"),
			bd.name, // button text
			WS_CHILD | WS_VISIBLE |
			BS_FLAT,
			buttonWidth * rowAdj,
			rc.bottom - buttonHeight * colAdj,
			buttonWidth,
			buttonHeight,
			hwnd,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (rowAdj == 2) {
			rowAdj = 0;
			++colAdj;
		}
		else
			++rowAdj;
	}

	CreateWindow(
		TEXT("Edit"),
		TEXT("\r\n\r\n"), // button text
		WS_CHILD | WS_BORDER | WS_VISIBLE |
		ES_MULTILINE | ES_CENTER | ES_READONLY | ES_AUTOVSCROLL,
		0,
		0,
		rc.right,
		rc.bottom / 6 * 2,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL);
}

std::int64_t result{};
char op{ '+' };
int num{}; // right operand
bool isNumNeeded{ false };
bool isDone{ false };

void doOp(char c, int n) {
	switch (c) {
	case '+':
		result += n; return;
	case '-':
		result -= n; return;
	}
}

bool getToken(char c) {
	if (isNumNeeded) {
		switch (c) {
		case '1': num = 1; return true;
		case '2': num = 2; return true;
		case '3': num = 3; return true;
		case '4': num = 4; return true;
		case '5': num = 5; return true;
		case '6': num = 6; return true;
		case '7': num = 7; return true;
		case '8': num = 8; return true;
		case '9': num = 9; return true;
		}
	}
	else {
		switch (c) {
		case '+': op = '+'; return true;
		case '-': op = '-'; return true;
		case '=': op = '='; return true;
		}
	}

	return false;
}

void procButtonInput(HWND hwndParent, WPARAM wParam, LPARAM lParam) {
	int notif = HIWORD(wParam);
	switch (notif) {
	case BN_CLICKED:
		HWND hbw = (HWND)lParam;
		TCHAR bwName[2]{};
		GetWindowText(hbw, bwName, 2);

		if (!getToken(bwName[0]))
			return;

		HWND hew = FindWindowEx(hwndParent, NULL, TEXT("Edit"), NULL);

		if (isDone) {
			SetWindowText(hew, TEXT("\r\n\r\n"));
			isDone = FALSE;
			op = '+';
			result = 0;
		}

		TCHAR ewName[1000]{};
		GetWindowText(hew, ewName, 1000);

		isNumNeeded = !isNumNeeded;

		if (op && num) {
			doOp(op, num);
			op = NULL;
			num = NULL;
		}
		if (op == '=') {
			TCHAR textResult[1000]{};
			wsprintf(textResult, TEXT("%s %s %lli"), ewName, bwName, result);
			SetWindowText(hew, textResult);
			isDone = TRUE;
			return;
		}

		TCHAR textResult[1000]{};
		wsprintf(textResult, TEXT("%s %s"), ewName, bwName);
		SetWindowText(hew, textResult);
	}
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_COMMAND:
		procButtonInput(hwnd, wParam, lParam);
		return 0;

	case WM_CREATE:
		initCalcButtons(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE _, PSTR szCmd, int nCmd) {
	InitCommonControls();

	const TCHAR* const MAIN_CLASS = TEXT("MainWindow");

	WNDCLASSEX wcx{};

	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hInstance = hInstance;
	wcx.lpszClassName = MAIN_CLASS;
	wcx.lpfnWndProc = MainProc;
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wcx);

	HWND hwnd = CreateWindow(
		MAIN_CLASS,
		TEXT("Calculator"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		GetSystemMetrics(SM_CXSCREEN) / 5.1, GetSystemMetrics(SM_CYSCREEN) / 2.4,
		NULL,
		NULL,
		hInstance,
		NULL);

	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}