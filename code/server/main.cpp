#include <iostream>
#include <yarp/os/all.h>
#include "FIFO.hpp"
#include "Server.hpp"

using namespace std;
using namespace yarp::os;


int main(int argc, char * argv[])
{
	Network yarp;		//initialize yarp
	Server ServModule;	//create module for maneging message and showing of the content of FIFO

	/* prepare and configure the resource finder */
	ResourceFinder rf;
	rf.configure(argc, argv);
	rf.setVerbose(true);
	cout << "Configuring and starting module. \n";
	ServModule.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule()

	return 0;
}