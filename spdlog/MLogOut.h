/******************************************************************************
作者：		何登锋
功能说明：
    使用spglog的日志单例
******************************************************************************/

#ifndef MLOGOUT_H
#define MLOGOUT_H

#include <io.h>
#include <direct.h>
#include <memory>

#include "spdlog/spdlog.h"

#include "MFrame/Singleton.h"
#include "MFrame/FrameCommon.h"

class MLogOut
{
public:

    ~MLogOut();

    bool check_dir(const char* dir);

    void info_out(const char * fmt, ...);

    void debug_out(const char * fmt, ...);

    void warn_out(const char * fmt, ...);

    void error_out(const char * fmt, ...);


private:
    MLogOut();
    DECLARE_SINGLETON_CLASS(MLogOut);

    std::shared_ptr<spdlog::logger> _logger;

//#ifdef QT_DEBUG
//    std::shared_ptr<spdlog::logger>	_console;
//#endif

};


typedef Singleton<MLogOut> MLogOutSingleton;

#define m_log_out (* reinterpret_cast<MLogOut*>(MLogOutSingleton::instance()))

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

#define III(msg,...) m_log_out.info_out(suffix(msg),__VA_ARGS__)
#define DDD(msg,...) m_log_out.debug_out(suffix(msg),__VA_ARGS__)



#endif // MLOGOUT_H
