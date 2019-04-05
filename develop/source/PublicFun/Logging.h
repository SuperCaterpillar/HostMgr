#ifndef LOGGING_H
#define LOGGING_H
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/serialization/singleton.hpp>

#include <string>

#include "Singleton.h"

namespace logging  = boost::log;
namespace src      = boost::log::sources;
namespace sinks    = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr     = boost::log::expressions;
namespace attrs    = boost::log::attributes;

using namespace  std;

class Logging : public Singleton<Logging>
{
private:
    Logging();
public:
    void set_file_full_path(const string &);
    void set_log_size(const int);
    void set_log_filter(logging::trivial::severity_level);
    void init_logging();
private:
    friend class Singleton<Logging>;
};

#define LogTrace      BOOST_LOG_TRIVIAL(trace)  <<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"
#define LogDebug      BOOST_LOG_TRIVIAL(debug)  <<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"
#define LogInfo       BOOST_LOG_TRIVIAL(info)   <<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"
#define LogWarning    BOOST_LOG_TRIVIAL(warning)<<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"
#define LogError      BOOST_LOG_TRIVIAL(error)  <<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"
#define LogFatal      BOOST_LOG_TRIVIAL(fatal)  <<"[fileName: "<<__FILE__<<" fun: "<<__func__<<" line: "<<__LINE__<< "]\n"



#endif // LOGGING_H
