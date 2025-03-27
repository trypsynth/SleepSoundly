#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "app.hpp"
#include "single_instance.hpp"
#include <stdexcept>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int) {
	try {
		single_instance inst("SleepSoundly");
		return app{instance}.run();
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
		return 1;
	}
}
