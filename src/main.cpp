#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "app.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int) {
	return app{instance}.run();
}
