#include "Logging.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <boost/filesystem.hpp>

_INITIALIZE_EASYLOGGINGPP

void init_logging(int argv, char* argc[])
{
    _START_EASYLOGGINGPP(argv, argc);

    // Make sure the logging directory exists
    boost::filesystem::path logDir("./logs");
    if (!boost::filesystem::exists(logDir) && !boost::filesystem::create_directory(logDir))
    {
        throw std::runtime_error("Failed to create the logs diretory!");
    }

    // Redirect stdout to a file
    freopen("logs/stdout.log", "w", stdout);

    // Set up logging
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Configurations conf;
    conf.setToDefault();

    // Determine log name
    time_t time = std::time(nullptr);
    tm* localTime = std::localtime(&time);

    std::stringstream timeStr;
    timeStr << std::put_time(localTime, "%Y-%m-%d");

    std::string filename = "./logs/log-" + timeStr.str() + ".log";

#ifdef CLEAN_LOG
    // Delete the log file.
    boost::filesystem::path logPath(filename);
    if (boost::filesystem::exists(logPath))
    {
        boost::filesystem::remove(logPath);
    }
#endif

    // Set log filename
    conf.setGlobally(el::ConfigurationType::Filename, filename);

    // Configure log format for each severity level
    std::string preamble = "%datetime{%Y-%M-%d %H:%m:%s:%g} [%levshort] %fbase:%line: ";
    conf.setGlobally(el::ConfigurationType::Format, preamble + "%msg");
    conf.set(el::Level::Debug, el::ConfigurationType::Format, preamble + "* %msg *");
    conf.set(el::Level::Trace, el::ConfigurationType::Format, preamble + "%msg");
    conf.set(el::Level::Info, el::ConfigurationType::Format, preamble + "=== %msg ===");
    conf.set(el::Level::Warning, el::ConfigurationType::Format, preamble + "@@@ %msg @@@");
    conf.set(el::Level::Error, el::ConfigurationType::Format, preamble + "!!! %msg !!!");
    conf.set(el::Level::Fatal, el::ConfigurationType::Format, preamble + "x_x %msg x_x");
    el::Loggers::reconfigureLogger("default", conf);

    LOG(INFO) << "Logging initialized";
}