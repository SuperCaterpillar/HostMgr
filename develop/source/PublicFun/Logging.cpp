#include "Logging.h"

Logging::Logging()
{
    logging::add_file_log(
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
                keywords::format = (expr::stream
                                    <<"["
                                    <<expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S")<<"] "
                                    <<"ProcessName:"<<expr::attr<std::string>("Process") <<" "
                                    <<"ThreadID:"<<expr::attr<attrs::current_thread_id::value_type>("ThreadID")<<"\n"
                                    <<expr::smessage)
            );
    //add time
    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    logging::core::get()->add_global_attribute("Process", attrs::current_process_name());

    //    logging::add_file_log(
    //                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
    //                keywords::format = (expr::stream
    //                                    <<"["
    //                                    <<expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S")<<"] "
    //                                    <<"ProcessName:"<<expr::attr<std::string>("Process") <<" "
    //                                    <<"ThreadID:"<<expr::attr<attrs::current_thread_id::value_type>("ThreadID")<<"\n"
    //                                    <<expr::smessage)
    //            );
    //    //add time
    //    logging::add_common_attributes();
    //    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    //    logging::core::get()->add_global_attribute("Process", attrs::current_process_name());
}

void Logging::set_file_full_path(const string & path)
{
    logging::add_file_log(keywords::file_name = path + "_%Y-%m-%d %H:%M:%S.log");
}

void Logging::set_log_size(const int size)
{
    logging::add_file_log( keywords::rotation_size = size);
}

void Logging::set_log_filter(boost::log::trivial::severity_level level)
{
    logging::core::get()->set_filter(logging::trivial::severity >= level);
}

void Logging::init_logging()
{
    //    logging::add_file_log(
    //                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
    //                keywords::format = (expr::stream
    //                                    <<"["
    //                                    <<expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S")<<"] "
    //                                    <<"ProcessName:"<<expr::attr<std::string>("Process") <<" "
    //                                    <<"ThreadID:"<<expr::attr<attrs::current_thread_id::value_type>("ThreadID")<<"\n"
    //                                    <<expr::smessage)
    //            );
    //    //add time
    //    logging::add_common_attributes();
    //    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    //    logging::core::get()->add_global_attribute("Process", attrs::current_process_name());

    //    logging::core::get()->set_filter(logging::trivial::severity >=logging::trivial::trace);
    logging::add_file_log(
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
                keywords::format = (expr::stream
                                    <<"["
                                    <<expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S")<<"] "
                                    <<"ProcessName:"<<expr::attr<std::string>("Process") <<" "
                                    <<"ThreadID:"<<expr::attr<attrs::current_thread_id::value_type>("ThreadID")<<"\n"
                                    <<expr::smessage)
            );
    //add time
    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    logging::core::get()->add_global_attribute("Process", attrs::current_process_name());
}
