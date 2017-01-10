#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "FIFO.hpp"
#include "Server.hpp"

using namespace std;
using namespace yarp::os;

double Server::getPeriod()
{
	return 1.0;     // call update module every 1 sec.
}

// This is our main function. Will be called periodically every getPeriod() seconds
bool Server::updateModule()
{
	fifo.show();
	return true;
}
    
// Message handler
bool Server::respond(const Bottle& botRequest, Bottle& botCommand)
{
	if(botRequest.get(0).asInt() == COLLATZ_VOCAB_REQ_ITEM){           //check the header of received messages
		sem.wait();
		if(botRequest.get(1).asInt() != 0){                     //first connection from client sends 0, so skip
			//cout << "received num=" << botRequest.get(1).asInt() << endl;
			fifo.delete_element(botRequest.get(1).asInt());     //delete the received element from FIFO
		}

		//increment and push back the natural N
		intCNT++;
		fifo.enqueue(intCNT);

		//make a command and send it
		botCommand.addInt(COLLATZ_VOCAB_ITEM);
		botCommand.addInt(intCNT);
		botCommand.addInt(fifo.head_value()-1);
		sem.post();
		return true;
	}
	else{
		cout << "receiced another message" << endl;
		return false;
	}
}

bool Server::configure(yarp::os::ResourceFinder &rf)
{
	intCNT=1;     //set initial value of the counter   
	handlerPort.open("/server");
	attach(handlerPort);	//messages received from the port are redirected to the respond method
	return true;
}

// Close function, to perform cleanup.
bool Server::close()
{
	//close port
	cout << "Calling close function\n";
	handlerPort.close();
	return true;
}