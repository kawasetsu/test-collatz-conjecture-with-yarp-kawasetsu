#ifndef __SERVER_HPP_INCLUDED__
#define __SERVER_HPP_INCLUDED__

#include <yarp/os/all.h>
#include "FIFO.hpp"
using namespace std;
using namespace yarp::os;

class Server:public RFModule
{
	RpcServer handlerPort;
	FIFO fifo;
	Semaphore sem;
	int intCNT;
public:
	bool configure(ResourceFinder &rf);
	double getPeriod();
	bool updateModule();
	bool respond(const Bottle& botResponse, Bottle& botCommand);
	bool close();
};

#endif