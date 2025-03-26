#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>
#include <algorithm>
#include "resource.h"

constexpr UINT ID_TRAY_ICON = 1001;
constexpr UINT WM_TRAYICON = WM_USER + 1;
constexpr const char* WND_CLASS_NAME = "SleepSoundlyWndClass";
constexpr const char* TRAY_TIP = "sleep_soundly";

HINSTANCE h_inst;
HWND h_wnd;
NOTIFYICONDATA nid{};

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void play_silence();

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int) {
	h_inst = instance;
	WNDCLASS wc{};
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = instance;
	wc.lpszClassName = WND_CLASS_NAME;
	RegisterClass(&wc);
	h_wnd = CreateWindowEx(0, WND_CLASS_NAME, TRAY_TIP, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, instance, nullptr);
	nid.cbSize = sizeof(nid);
	nid.hWnd = h_wnd;
	nid.uID = ID_TRAY_ICON;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	std::copy(TRAY_TIP, TRAY_TIP + sizeof(nid.szTip), nid.szTip);
	Shell_NotifyIcon(NIM_ADD, &nid);
	play_silence();
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_TRAYICON:
			if (LOWORD(lp) == WM_RBUTTONUP) {
				Shell_NotifyIcon(NIM_DELETE, &nid);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

void play_silence() {
	HRSRC res = FindResource(h_inst, MAKEINTRESOURCEA(IDR_WAVE1), "WAVE");
	if (!res) return;
	HGLOBAL res_handle = LoadResource(h_inst, res);
	if (!res_handle) return;
	void* res_data = LockResource(res_handle);
	DWORD res_size = SizeofResource(h_inst, res);
	PlaySound(static_cast<LPCSTR>(res_data), nullptr, SND_MEMORY | SND_LOOP | SND_ASYNC);
}
