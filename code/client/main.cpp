#include <iostream>
#include <yarp/os/all.h>
#include "Client.hpp"

using namespace std;
using namespace yarp::os;

int main(int argc, char * argv[])
{
	if(argc<2){
		cout << "Please supply a port name of client, for example: --name /c1" << endl;
		return 1;
	}

	//instantiate client module for maneging message
	Client clieModule;

	//prepare and configure the resource finder
	ResourceFinder rf;
	rf.configure(argc, argv);
	rf.setVerbose(true);
	cout << "Configuring and starting module" << endl;
	//run the module
	clieModule.runModule(rf);

	return 0;
}