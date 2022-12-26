#include "MainDlg.h"
#include <thread>
#include <string>
#include <time.h>
#pragma comment(lib, "winmm.lib")

const short FIELD_SIZE = 64;
HANDLE hMutex;
HWND Buttons[FIELD_SIZE];
HWND EditMines;
HWND EditTime;
DWORD IDC_TIMER;
HBITMAP hBM[10];
HBITMAP hQuestion;
HINSTANCE hInst;
unsigned short numbers[FIELD_SIZE];
unsigned short marks[FIELD_SIZE];
unsigned short points;
bool mines[FIELD_SIZE];
bool is_first_click = 1;
bool opened[FIELD_SIZE];
unsigned short flags = 0;
unsigned int GameTime = 0;
//WNDPROC oldButtonProc;

HWND MainWnd;
void OpenNullField(short num);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow);
int MyRandom(int min, int max);
void GetPlace(HWND hWnd, int& x, int& y);
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HHOOK mouseHook;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) noexcept
{
	hInst = hInstance;
	srand(time(0));
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	Dlg dlg;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg::DlgProc);
}

Dlg* Dlg::ptr = NULL;

Dlg::Dlg(void)
{
	ptr = this;
}

void GetPlace(HWND hWnd, int &x, int &y) {
	POINT p;
	ScreenToClient(hWnd, &p);
	x = p.x;
	y = p.y;
}

//void subclassButton(HWND hButtonWnd)
//{
//	// Преобразуем типы без нарушения strict aliasing rule
//	const WNDPROC newProc = WindowProc;
//	LONG_PTR newValue;
//#if __cplusplus >= 201103L
//	static_assert(sizeof(newValue) == sizeof(newProc), "incompatible conversion");
//#endif
//	memcpy(&newValue, &newProc, sizeof(newProc));
//
//	const LONG_PTR oldValue = SetWindowLongPtr(hButtonWnd, GWLP_WNDPROC, newValue);
//
//#if __cplusplus >= 201103L
//	static_assert(sizeof(oldButtonProc) == sizeof(oldValue), "incompatible conversion");
//#endif
//	memcpy(&oldButtonProc, &oldValue, sizeof(oldValue));
//}

void OpenNullField(short num) {
	opened[num] = 1;
	SendMessage(Buttons[num], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[0]);
	if (num % 8 != 0 && !opened[num - 1] && marks[num - 1] == 0) {
		points++;
		if (numbers[num - 1] == 0) OpenNullField(num - 1);
		else SendMessage(Buttons[num - 1], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num - 1]]), opened[num - 1] = 1;
	}
	if (num % 8 != 7 && !opened[num + 1] && marks[num + 1] == 0) {
		points++;
		if (numbers[num + 1] == 0) OpenNullField(num + 1);
		else SendMessage(Buttons[num + 1], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num + 1]]), opened[num + 1] = 1;
	}
	if (num - 7 >= 8 && !opened[num - 8] && marks[num - 8] == 0) {
		points++;
		if (numbers[num - 8] == 0) OpenNullField(num - 8);
		else SendMessage(Buttons[num - 8], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num - 8]]), opened[num - 8] = 1;
	}
	if (num + 8 <= FIELD_SIZE - 1 && !opened[num + 8] && marks[num + 8] == 0) {
		points++;
		if (numbers[num + 8] == 0) OpenNullField(num + 8);
		else SendMessage(Buttons[num + 8], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num + 8]]), opened[num + 8] = 1;
	}
	if (num % 8 != 0 && num - 8 >= 8 && !opened[num - 9] && marks[num - 9] == 0) {
		points++;
		if (numbers[num - 9] == 0) OpenNullField(num - 9);
		else SendMessage(Buttons[num - 9], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num - 9]]), opened[num - 9] = 1;
	}
	if (num % 8 != 0 && num + 8 <= FIELD_SIZE - 1 && !opened[num + 7] && marks[num + 7] == 0) {
		points++;
		if (numbers[num + 7] == 0) OpenNullField(num + 7);
		else SendMessage(Buttons[num + 7], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num + 7]]), opened[num + 7] = 1;
	}
	if (num % 8 != 7 && num + 8 <= FIELD_SIZE - 1 && !opened[num + 9] && marks[num + 9] == 0) {
		points++;
		if (numbers[num + 9] == 0) OpenNullField(num + 9);
		else SendMessage(Buttons[num + 9], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num + 9]]), opened[num + 9] = 1;
	}
	if (num % 8 != 7 && num - 8 >= 0 - 1 && !opened[num - 7] && marks[num - 7] == 0) {
		points++;
		if (numbers[num - 7] == 0) OpenNullField(num - 7);
		else SendMessage(Buttons[num - 7], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[num - 7]]), opened[num - 7] = 1;
	}
}

int MyRandom(int min, int max) {
	return rand() % (max - min + 1) + min;
}

//static void onRButtonClick(HWND hwnd, BOOL bDblClick, int x, int y, UINT uKey)
//{
//	MessageBox(0,0,0,0);
//}

//static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (uMsg == HC_ACTION) {
//		switch (wParam)
//		{
//		case WM_RBUTTONUP:
//			HANDLE_MSG(MainWnd, WM_RBUTTONDOWN, onRButtonClick);
//		default:
//			break;
//		}
//		return 0;
//	}
//
//	return CallNextHookEx(NULL, uMsg, wParam, lParam);
//}

BOOL CALLBACK Dlg::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		hMutex = CreateMutex(NULL, TRUE, L"ApplicationName");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, L"Application is already running!", L"Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		SetTimer(hWnd, IDC_TIMER, 100, 0);

		flags = 0;
		GameTime = 0;
		is_first_click = 1;
		points = 0;

		hBM[0] = (HBITMAP)LoadImage(NULL, L"BMP/field0.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[1] = (HBITMAP)LoadImage(NULL, L"BMP/field1.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[2] = (HBITMAP)LoadImage(NULL, L"BMP/field2.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[3] = (HBITMAP)LoadImage(NULL, L"BMP/field3.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[4] = (HBITMAP)LoadImage(NULL, L"BMP/field4.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[5] = (HBITMAP)LoadImage(NULL, L"BMP/field5.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[6] = (HBITMAP)LoadImage(NULL, L"BMP/field6.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[7] = (HBITMAP)LoadImage(NULL, L"BMP/field7.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[8] = (HBITMAP)LoadImage(NULL, L"BMP/field8.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[9] = (HBITMAP)LoadImage(NULL, L"BMP/field9.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hBM[10] = (HBITMAP)LoadImage(NULL, L"BMP/field10.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hQuestion = hBM[10];

		EditMines = GetDlgItem(hWnd, IDC_EDIT1);
		EditTime = GetDlgItem(hWnd, IDC_EDIT2);
		EnableWindow(EditMines, 0);
		EnableWindow(EditTime, 0);

		for (int i = 0; i < FIELD_SIZE; i++) {
			Buttons[i] = GetDlgItem(hWnd, IDC_BUTTON2 + i);
			opened[i] = 0;
			marks[i] = 0;
			// subclassButton(Buttons[i]);
			SendMessage(Buttons[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[8]);
		}
	}
	break;

	case WM_CLOSE:
	{
		UnhookWindowsHookEx(mouseHook);
		EndDialog(hWnd, 0);
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
	break;

	case WM_TIMER: {
		std::wstring s = std::to_wstring((int)((GameTime / 10) % 60)).c_str();
		std::wstring m = std::to_wstring((int)((GameTime / 10) / 60 % 60)).c_str();
		std::wstring h = std::to_wstring((int)((GameTime / 10) / 3600)).c_str();
		std::wstring dblPoint = L":";
		std::wstring res = h + dblPoint + m + dblPoint + s;
		SetDlgItemText(hWnd, IDC_EDIT1, res.c_str());
		res = std::to_wstring(flags).c_str() + std::wstring(L" / 9");
		SetDlgItemText(hWnd, IDC_EDIT2, res.c_str());
		GameTime++;
		//SetDlgItemText(hWnd, IDC_EDIT2, std::to_wstring(points).c_str());
	}
	break;

	case WM_CONTEXTMENU:
	{
			WORD x, y;
			WORD xPos = GET_X_LPARAM(lParam), yPos = GET_Y_LPARAM(lParam);
			RECT rc;
			GetWindowRect(hWnd, &rc);
			x = xPos - rc.left, y = yPos - rc.top;

			unsigned short res = 0;

		for (int i = 0; i < FIELD_SIZE; i++) {
			RECT ButtonBorders;
			GetWindowRect(Buttons[i], &ButtonBorders);
			WORD ButtonL = ButtonBorders.left - rc.left,
				 ButtonR = ButtonBorders.right - rc.right,
				 ButtonT = ButtonBorders.top - rc.top,
				 ButtonB = ButtonBorders.bottom - rc.bottom;

			if (ButtonL < x && x < ButtonR && ButtonT < y && y < ButtonB) res = i;
		}
		if (!opened[res]) switch (marks[res]) {
		case 0:
			SendMessage(Buttons[res], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[9]);
			marks[res] = 1;
			flags++;
			break;

		case 1:
			SendMessage(Buttons[res], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hQuestion);
			marks[res] = 2;
			flags--;
			break;

		case 2:
			SendMessage(Buttons[res], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[8]);
			marks[res] = 0;
			break;
		}
	}
	break;


	case WM_COMMAND:
	{
		if (IDC_BUTTON2 <= wParam && wParam <= IDC_BUTTON65) {
			short ClickPlace = wParam - IDC_BUTTON2;
			if (is_first_click) {
				is_first_click = 0;
				for (int i = 0; i < FIELD_SIZE; i++) opened[i] = 0;
				bool is_first_cycle = 1;
				do {
					if (is_first_click) {
						is_first_cycle = 0;
					}
					else {
						for (int i = 0; i < FIELD_SIZE; i++) {
							numbers[i] = 0;
							mines[i] = 0;
						}
					}
					for (int i = 0; i < 9; i++) {
						unsigned short res = MyRandom(0, FIELD_SIZE - 1);
						if (mines[res] == 0) mines[res] = 1;
						else i--;
					}

					for (int i = 0; i < FIELD_SIZE; i++) {
						if (mines[i]) {
							numbers[i] = 7;
							continue;
						}
						int res = 0;
						if (mines[i - 1] && i % 8 != 0)									res++;
						if (mines[i + 1] && i % 8 != 7)									res++;
						if (mines[i - 8] && i - 8 >= 0)									res++;
						if (mines[i + 8] && i + 8 <= FIELD_SIZE - 1)					res++;
						if (mines[i - 9] && i % 8 != 0 && i - 8 >= 0)					res++;
						if (mines[i - 7] && i % 8 != 7 && i - 8 >= 0)					res++;
						if (mines[i + 9] && i % 8 != 7 && i + 8 <= FIELD_SIZE - 1)		res++;
						if (mines[i + 7] && i % 8 != 0 && i + 8 <= FIELD_SIZE - 1)		res++;
						numbers[i] = res;
					}

				} while (numbers[ClickPlace] != 0);
			}
			if (numbers[ClickPlace] == 0 && !opened[ClickPlace] && marks[ClickPlace] == 0) {
				points++;
				PlaySound(TEXT("sound0.wav"), NULL, SND_FILENAME | SND_ASYNC);
				OpenNullField(ClickPlace);
			}
			else if (!opened[ClickPlace] && marks[ClickPlace] == 0) {
				opened[ClickPlace] = 1;
				SendMessage(Buttons[ClickPlace], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[numbers[ClickPlace]]);
				if (numbers[ClickPlace] == 7) {
					PlaySound(TEXT("BOOM.wav"), NULL, SND_FILENAME | SND_ASYNC);
					for (int i = 0; i < FIELD_SIZE; i++) {
						if (numbers[i] == 7) {
							SendMessage(Buttons[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM[7]);
						}
					}
					short res = MessageBox(0, L"Вы проиграли\nХотите попробоать ещё раз?", 0, MB_YESNO | MB_ICONQUESTION);
					switch (res)				{
					case IDYES:
						ShowWindow(FindWindow(0, L"Minesweaper"), SW_HIDE);
						DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg::DlgProc, 0);
					case IDNO:
						EndDialog(hWnd, 0);
						break;
					}
				}
				points++;
			}
			if (points >= 54) {
				PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC);
				short res = MessageBox(0, L"Вы проиграли\nХотите попробоать ещё раз?", 0, MB_YESNO | MB_ICONQUESTION);
				switch (res) {
				case IDYES:
					ShowWindow(FindWindow(0, L"Minesweaper"), SW_HIDE);
					DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg::DlgProc, 0);
				case IDNO:
					EndDialog(hWnd, 0);
					break;
				}
			}
		}
		else switch (wParam)
		{

		}
	}
	break;

	}
	return FALSE;
}