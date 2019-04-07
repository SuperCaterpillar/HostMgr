#include "LoadSubModules.h"
#include "../PublicFun/Logging.h"
#include "../PublicFun/Timer.h"
#include <boost/bind.hpp>
#include "../KernelControl/GenetlinkSocket.h"
#include "../PublicFun/NetLinkKernel.h"

int main()
{
    Logging::instance()->init_logging();
    //Logging::instance()->set_file_full_path("../log/Run");
    LoadSubModules l;



    l.StartAllModules();


    while(1)
    {
        sleep(20);

        GenetlinkSocket genetlink_socket;
        if (!genetlink_socket.Init(HOSTMGR_GENL_FAMILY_NAME))
        {
            cout <<("StartKernelMod@Init ERROR");
        }

    }

}
