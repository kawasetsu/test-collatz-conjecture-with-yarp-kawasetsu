#ifndef __RFMC_HPP_INCLUDED__
#define __RFMC_HPP_INCLUDED__

#include <yarp/os/all.h>

class MyModule:public RFModule
{
    Port handlerPort; // a port to handle messages
    int count;
public:
    bool respond(const Bottle& botResponse, Bottle& botCommand);
    bool configure(yarp::os::ResourceFinder &rf);
    bool interruptModule();
    bool close();
};

#endif