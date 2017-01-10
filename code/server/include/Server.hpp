#ifndef __SERVER_HPP_INCLUDED__
#define __SERVER_HPP_INCLUDED__

#include <yarp/os/all.h>
#include "FIFO.hpp"
using namespace std;
using namespace yarp::os;

class Server:public RFModule
{
	RpcServer handlerPort; // a port to handle messages
	FIFO Fifo;
	Semaphore Sem;
	int intCNT;
public:
	double getPeriod();
	bool updateModule();
	bool respond(const Bottle& botResponse, Bottle& botCommand);
	bool configure(yarp::os::ResourceFinder &rf);
	bool close();
};

#endif