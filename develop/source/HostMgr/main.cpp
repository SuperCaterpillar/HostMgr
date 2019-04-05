#include "LoadSubModules.h"
#include "../PublicFun/Logging.h"
#include "../PublicFun/Timer.h"
#include <boost/bind.hpp>


int main()
{
    Logging::instance()->init_logging();
    //Logging::instance()->set_file_full_path("../log/Run");
    LoadSubModules l;



    l.StartAllModules();


    while(1)
    {
        sleep(20);
    }

}
