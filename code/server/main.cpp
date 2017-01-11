#include <iostream>
#include <yarp/os/all.h>
#include "FIFO.hpp"
#include "Server.hpp"

using namespace std;
using namespace yarp::os;

int main(int argc, char * argv[])
{
	//initialize yarp and instantiate server module for maneging message and showing of the content of FIFO
	Network yarp;
	Server servModule;

	//prepare and configure the resource finder
	ResourceFinder rf;
	rf.configure(argc, argv);
	rf.setVerbose(true);
	cout << "Configuring and starting module" << endl;
	//run the module
	servModule.runModule(rf);

	return 0;
}