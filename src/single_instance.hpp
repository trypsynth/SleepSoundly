#pragma once

#include <string>
#include <windows.h>

class single_instance {
public:
	explicit single_instance(const std::string& app_id);
	~single_instance();

private:
	HANDLE handle_;
};
