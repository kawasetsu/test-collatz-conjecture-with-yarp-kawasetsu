#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "FIFO.hpp"
#include "Server.hpp"

using namespace std;
using namespace yarp::os;

bool Server::configure(ResourceFinder &rf)
{
	//set initial value of the counter  
	intCNT=1; 

	//messages received from the port are redirected to the respond method
	handlerPort.open("/server");
	attach(handlerPort);
	return true;
}

double Server::getPeriod()
{
	// call update module every 1 sec.
	return 1.0;
}

bool Server::updateModule()
{
	// print pending numbers in FIFO
	fifo.show();
	return true;
}

bool Server::respond(const Bottle& botRequest, Bottle& botCommand)
{
	//check the header of received messages
	if(botRequest.get(0).asInt() == COLLATZ_VOCAB_REQ_ITEM){
		//to avoid conflict of data access
		sem.wait();
		//first connection from client sends 0, so skip
		if(botRequest.get(1).asInt() != 0){
			//delete the received element from FIFO
			fifo.delete_element(botRequest.get(1).asInt());
		}

		//increment and push back the positive integer N
		intCNT++;
		fifo.enqueue(intCNT);

		//make a command and send it to client
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

bool Server::close()
{
	//close port
	cout << "Calling close function" << endl;
	handlerPort.close();
	return true;
}