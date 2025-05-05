#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include "main.h"

HINSTANCE h_inst;
HANDLE mutex;
HWND hwnd;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void show_tray_menu();
#pragma function(memset)
void* __cdecl memset(void* dest, int value, size_t count);

int main() {
	h_inst = GetModuleHandle(NULL);
	mutex = CreateMutex(NULL, TRUE, L"SleepSoundly_IsAlreadyRunning");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(GetActiveWindow(), L"Another instance of SleepSoundly is already running.", L"Error", MB_ICONERROR);
		return 1;
	}
	WNDCLASS wc = {0};
	wc.style = 0;
	wc.lpfnWndProc = wnd_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = h_inst;
	wc.lpszClassName = L"SleepSoundlyWndClass";
	RegisterClass(&wc);
	hwnd = CreateWindowEx(0, L"SleepSoundlyWndClass", L"Sleep Soundly", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, h_inst, NULL);
	NOTIFYICONDATA nid = {0};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uID = ID_TRAY_ICON;
	nid.uFlags = NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	const wchar_t* tip = L"Sleep Soundly";
	int i = 0;
	while (i < sizeof(nid.szTip) / sizeof(wchar_t) - 1 && tip[i] != L'\0') {
		nid.szTip[i] = tip[i];
		++i;
	}
	nid.szTip[i] = L'\0';
	Shell_NotifyIcon(NIM_ADD, &nid);
	PlaySound(MAKEINTRESOURCE(IDR_SILENCE), h_inst, SND_RESOURCE | SND_LOOP | SND_ASYNC);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Shell_NotifyIcon(NIM_DELETE, &nid);
	if (mutex) {
		ReleaseMutex(mutex);
		CloseHandle(mutex);
	}
	return 0;
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_TRAYICON:
			if (LOWORD(lp) == WM_RBUTTONUP) show_tray_menu();
			break;
		case WM_COMMAND:
			if (LOWORD(wp) == ID_EXIT) PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

void show_tray_menu() {
	HMENU menu = CreatePopupMenu();         
	if (menu) {
		AppendMenu(menu, MF_STRING, ID_EXIT, L"E&xit");
		POINT pt;
		GetCursorPos(&pt);
		SetForegroundWindow(hwnd);
		TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
		DestroyMenu(menu);
	}
}

// Allows for {0} initialization without the CRT.
void* __cdecl memset(void* dest, int value, size_t count) {
	unsigned char* p = dest;
	while (count--) *p++ = (unsigned char)value;
	return dest;
}
