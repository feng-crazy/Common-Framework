#include "MLogOut.h"

MLogOut::MLogOut()
{
    check_dir("logs");
    std::vector<spdlog::sink_ptr> sinks;
    //设置为异步日志，同步纯文件日志 3W/s，控制台日志 3k/s
    //spdlog::set_async_mode(32768);  // 必须为 2 的幂
#ifdef QT_DEBUG
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
#endif
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/log", 0, 0));

    _logger = std::make_shared<spdlog::logger>("both", begin(sinks), end(sinks));

    //register it if you need to access it globally
    spdlog::register_logger(_logger);

    // 设置日志记录级别
#ifdef QT_DEBUG
    _logger->set_level(spdlog::level::debug);
#else
    _logger->set_level(spdlog::level::warn);
#endif
    //设置 logger 格式
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][thread %t] %v ");
    //设置当出发 err 或更严重的错误时立刻刷新日志到  disk
    _logger->flush_on(spdlog::level::err);

}

MLogOut::~MLogOut()
{
    spdlog::drop_all();
}

bool MLogOut::check_dir(const char *dir)
{
    if (_access(dir, 0) == -1)
    {
        int flag = _mkdir(dir);

        return (flag == 0);
    }
    return true;
}

void MLogOut::info_out(const char * fmt, ...)
{
    char tmp[2048] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf_s(tmp, fmt, args);
    va_end(args);
    _logger->info(tmp);
}

void MLogOut::debug_out(const char * fmt, ...)
{
    char tmp[2048] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf_s(tmp, fmt, args);
    va_end(args);
    _logger->debug(tmp);
}

void MLogOut::warn_out(const char * fmt, ...)
{
    char tmp[2048] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf_s(tmp, fmt, args);
    va_end(args);
    _logger->warn(tmp);
}

void MLogOut::error_out(const char * fmt, ...)
{
    char tmp[2048] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf_s(tmp, fmt, args);
    va_end(args);
    _logger->warn(tmp);
}
