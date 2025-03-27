#include "single_instance.hpp"
#include <stdexcept>

single_instance::single_instance(const std::string& app_id) {
	std::string mutex_name = app_id + "_IsAlreadyRunning";
	handle_ = CreateMutex(nullptr, TRUE, mutex_name.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS) throw std::runtime_error{"Another instance is already running"};
}

single_instance::~single_instance() {
	if (handle_) CloseHandle(handle_);
}
