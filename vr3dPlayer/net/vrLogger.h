#ifndef _VR_LOGGER_H_
#define _VR_LOGGER_H_

#include <fstream>	//for std::ofstream
#include <iostream>	//for std::cerr
#include <mutex>	//for std::unique_lock<std::mutex>
#include <queue>	//for std::queue
namespace vr
{
	enum vrPriority
	{
		vrLogBug, vrLogState, vrLogInfo, vrLogWarning, vrLogError,
	};

	class vrLogger
	{
	public:
		vrLogger& operator=(const vrLogger &) = delete;	//��ֹʹ�ñ�����Ĭ�����ɵĸ�ֵ���캯��
		vrLogger(const vrLogger&) = delete;				//��ֹʹ�ñ�����Ĭ�����ɵĿ������캯��

		// ����ģʽ����ʵ��
		static vrLogger& instance();
		~vrLogger();

		void setLogFile(const char* pathname);
		void log(vrPriority priority, const char* __file, const char* __func, int __line, const char* fmt, ...);
		void log2(vrPriority priority, const char* fmt, ...);

	private:
		vrLogger();
		void run();

		std::ofstream _ofs;
		std::mutex    _mutex;
		std::queue<std::string> _queue;
		std::condition_variable _cond;
	};
}

#endif // !_VR_LOGGER_H_

