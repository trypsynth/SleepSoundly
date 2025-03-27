#pragma once

#include <string_view>
#include <windows.h>
#include <shellapi.h>

constexpr UINT ID_TRAY_ICON = 1001;
constexpr UINT WM_TRAYICON = WM_USER + 1;
constexpr UINT ID_EXIT = 2001;
constexpr std::string_view WND_CLASS_NAME = "sleep_soundly_wnd_class";
constexpr std::string_view TRAY_TIP = "Sleep Soundly";

class app {
public:
	explicit app(HINSTANCE instance);
	~app();
	int run();

private:
	HINSTANCE h_inst;
	HWND h_wnd{};
	NOTIFYICONDATA nid{};

	static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void show_tray_menu();
	void play_silence();
};
