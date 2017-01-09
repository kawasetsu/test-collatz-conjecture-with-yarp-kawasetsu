#ifndef __RFMS_HPP_INCLUDED__
#define __RFMS_HPP_INCLUDED__

#include <yarp/os/all.h>
#include "fifo.hpp"
using namespace std;
using namespace yarp::os;

class MyModule:public RFModule
{
    RpcServer handlerPort; // a port to handle messages
	FIFO Fifo;
	int intCNT;
public:
    double getPeriod();
    bool updateModule();
    bool respond(const Bottle& botResponse, Bottle& botCommand);
    bool configure(yarp::os::ResourceFinder &rf);
    bool close();
};

#endif