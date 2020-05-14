#include <stdarg.h>	//for va_start

#include "vrLogger.h"

using namespace vr;

const char* vrPriority_to_string[] =
{
	"DEBUG",
	"CONFIG",
	"INFO",
	"WARNING",
	"ERROR"
};

vrLogger& vrLogger::instance()
{
	static vrLogger s_logger;
	return s_logger;
}

vrLogger::vrLogger()
{
}

void vrLogger::run()
{
	std::unique_lock<std::mutex> lock(_mutex);
}

vrLogger::~vrLogger()
{
}
void vrLogger::setLogFile(const char* pathname)
{
	_ofs.open(pathname);
	if (_ofs.fail())
	{
		std::cerr << "failed to open logfile." << std::endl;
	}
}

void vrLogger::log(vrPriority priority, const char* __file, const char* __func, int __line, const char* fmt, ...)
{
	char buf[2048] = { 0 };

	sprintf(buf, "[%s][%s:%s:%d] ", vrPriority_to_string[priority], __file, __func, __line);
	va_list args;
	va_start(args, fmt);
	vsprintf(buf + strlen(buf), fmt, args);
	va_end(args);

	std::string entry(buf);
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
}

void vrLogger::log2(vrPriority priority, const char* fmt, ...)
{
	char buf[2048] = { 0 };

	sprintf(buf, "[%s] ", vrPriority_to_string[priority]);
	va_list args;
	va_start(args, fmt);
	vsprintf(buf + strlen(buf), fmt, args);
	va_end(args);

	std::string entry(buf);
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push(std::move(entry));
	_cond.notify_all();
}
