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

	Client clieModule;	//create module for maneging message

	/* prepare and configure the resource finder */
	ResourceFinder rf;
	rf.configure(argc, argv);
	rf.setVerbose(true);
	cout << "Configuring and starting module. \n";
	clieModule.runModule(rf);   // This calls configure(rf) and, upon success, the module execution begins with a call to updateModule()

	return 0;
}