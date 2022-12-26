#pragma once
#include "header.h"

class Dlg
{
public:
	Dlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Dlg* ptr;
};
