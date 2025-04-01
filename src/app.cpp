#include <algorithm>
#include "app.hpp"
#include <memory>
#include "resource.h"

app::app(HINSTANCE instance) :h_inst{instance} {
	WNDCLASS wc{
		.lpfnWndProc = wnd_proc,
		.hInstance = h_inst,
		.lpszClassName = WND_CLASS_NAME.data()
	};
	RegisterClass(&wc);
	h_wnd = CreateWindowEx(0, WND_CLASS_NAME.data(), TRAY_TIP.data(), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, h_inst, this);
	nid = {
		.cbSize = sizeof(nid),
		.hWnd = h_wnd,
		.uID = ID_TRAY_ICON,
		.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP,
		.uCallbackMessage = WM_TRAYICON,
		.hIcon = LoadIcon(nullptr, IDI_APPLICATION)
	};
	std::copy_n(TRAY_TIP.begin(), TRAY_TIP.size(), nid.szTip);
	Shell_NotifyIcon(NIM_ADD, &nid);
	play_silence();
}

app::~app() {
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

int app::run() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK app::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_CREATE) {
		auto* p_create = reinterpret_cast<LPCREATESTRUCT>(lp);
		auto* self = static_cast<app*>(p_create->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
	}
	if (auto* self = reinterpret_cast<app*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); self) {
		switch (msg) {
			case WM_TRAYICON:
				if (LOWORD(lp) == WM_RBUTTONUP) self->show_tray_menu();
				break;
			case WM_COMMAND:
				if (LOWORD(wp) == ID_EXIT) PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
	return 0;
}

void app::show_tray_menu() {
	auto h_menu = std::unique_ptr<std::remove_pointer_t<HMENU>, decltype(&DestroyMenu)>(CreatePopupMenu(), DestroyMenu);
	AppendMenu(h_menu.get(), MF_STRING, ID_EXIT, "Exit");
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(h_wnd);
	TrackPopupMenu(h_menu.get(), TPM_RIGHTBUTTON, pt.x, pt.y, 0, h_wnd, nullptr);
}

void app::play_silence() {
	if (auto res = FindResource(h_inst, MAKEINTRESOURCEA(IDR_WAVE1), "WAVE"); res) {
		if (auto res_handle = LoadResource(h_inst, res); res_handle) {
			if (void* res_data = LockResource(res_handle); res_data) {
				if (DWORD res_size = SizeofResource(h_inst, res); res_size) PlaySound(static_cast<LPCSTR>(res_data), nullptr, SND_MEMORY | SND_LOOP | SND_ASYNC);
			}
		}
	}
}
