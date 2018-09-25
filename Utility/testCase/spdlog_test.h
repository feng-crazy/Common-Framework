//  static function check dir 
// Check only one layer
#ifndef _CHECK_DIR_
#define _CHECK_DIR_

#ifdef WIN32  
#include <io.h>
#include <direct.h>  
#endif  
#ifdef linux   
#include <unistd.h> 
#include <sys/types.h>  
#include <sys/stat.h> 
#endif  

static bool check_dir(const char* dir)
{
	if (_access(dir, 0) == -1)
	{
#ifdef WIN32  
		int flag = _mkdir(dir);
#endif  
#ifdef linux   
		int flag = mkdir(dir.c_str(), 0777);
#endif  
		return (flag == 0);
	}
	return true;
};
#endif



//*****************************************************
//注意：
//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
//并非C++头文件中定义的 
//前两个变量是string类型，且__LINE__是整形，所以需要转为string类型
//******************************************************

//整数类型文件行号 ->转换为string类型  std::to_string 处理
#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#endif

//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#ifndef suffix
#define suffix(msg)  std::string(msg).append("  //")\
        .append(__FILENAME__).append(":").append(__func__)\
        .append("()#").append(std::to_string(__LINE__))\
        .append(".").c_str()
#endif

//在  spdlog.h   之前定义，才有效
#ifndef SPDLOG_TRACE_ON
#define SPDLOG_TRACE_ON
#endif

#ifndef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG_ON
#endif


//引入 log 头文件
#include <spdlog\spdlog.h>

// spdlog 封装
#ifndef _MY_LOG_WRAPPER_H_
#define _MY_LOG_WRAPPER_H_

class M_LOG_LIB
{

public:
	M_LOG_LIB()
	{
		check_dir("logs");
		std::vector<spdlog::sink_ptr> sinks;
		//设置为异步日志，同步纯文件日志 3W/s，控制台日志 3k/s
		//spdlog::set_async_mode(32768);  // 必须为 2 的幂
#ifdef _CONSOLE
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif
		sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log", 0, 0));
		//sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("logs/file.log"));
		nml_logger = std::make_shared<spdlog::logger>("both", begin(sinks), end(sinks));
		//register it if you need to access it globally
		spdlog::register_logger(nml_logger);

		// 设置日志记录级别
#ifdef _DEBUG
		nml_logger->set_level(spdlog::level::trace);
#else
		nml_logger->set_level(spdlog::level::err);
#endif
		//设置 logger 格式
		nml_logger->set_pattern("%T.%e  %t  [%L]  %v");
		//设置当出发 err 或更严重的错误时立刻刷新日志到  disk 
		nml_logger->flush_on(spdlog::level::err);

		//trd_logger = spdlog::daily_logger_mt("logs/trade", 0, 0);
	};
	inline ~M_LOG_LIB()
	{
		spdlog::drop_all();
	};
	inline auto getLogger() { return nml_logger; }
	void Out(const char * fmt, ...)
	{
		char tmp[2048] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf_s(tmp, fmt, args);
		va_end(args);
		nml_logger->info(tmp);
	};

	//目前只有一个 logger 需要增加交易 logger
	std::shared_ptr<spdlog::logger> nml_logger;
	//std::shared_ptr<spdlog::logger> trd_logger;
};
#endif

//如果没有生成对象，则生成并定义相关宏
//否则标记其为外部对象，直接使用
//可在整个工程里面  每个 cpp 里面包含该头文件
#ifndef _MY_OUTLOG_OBJECT_
#define _MY_OUTLOG_OBJECT_
extern M_LOG_LIB OutLog;
#define TTT(msg,...) OutLog.getLogger()->trace(suffix(msg),__VA_ARGS__)
#define DDD(msg,...) OutLog.getLogger()->debug(suffix(msg),__VA_ARGS__)
#define EEE(...) OutLog.getLogger()->error(__VA_ARGS__)
#define WWW(...) OutLog.getLogger()->warn(__VA_ARGS__)
#define III(...) OutLog.getLogger()->info(__VA_ARGS__)
#define CCC(...) OutLog.getLogger()->critical(__VA_ARGS__)
#endif
