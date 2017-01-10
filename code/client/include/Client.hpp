#ifndef __CLIENT_HPP_INCLUDED__
#define __CLIENT_HPP_INCLUDED__

#include <yarp/os/all.h>
using namespace std;
using namespace yarp::os;

class Client:public RFModule
{
	Network yarp;	//initialize yarp
	RpcClient handlerPort; // a port to handle messages
	int intN;
	int intTh;
public:
	double getPeriod();
	bool updateModule();
	bool respond(const Bottle& botResponse, Bottle& botCommand);
	bool configure(yarp::os::ResourceFinder &rf);
	bool close();
};

#endif