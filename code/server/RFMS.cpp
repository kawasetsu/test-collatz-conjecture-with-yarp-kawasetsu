#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "fifo.hpp"
#include "RFMS.hpp"

using namespace std;
using namespace yarp::os;

double MyModule::getPeriod()
{
    return 5.0;     // module periodicity (seconds), called implicitly by the module.
}

// This is our main function. Will be called periodically every getPeriod() seconds
bool MyModule::updateModule()
{
    Fifo.FIFO_show();
    return true;
}
    
// Message handler. Just echo all received messages.
bool MyModule::respond(const Bottle& botRequest, Bottle& botCommand)
{
    if(botRequest.get(0).asInt() == COLLATZ_VOCAB_REQ_ITEM){           //check the header of received messages
        if(botRequest.get(1).asInt() != 0){                     //first connection from client sends 0, so skip
            Fifo.delete_element(botRequest.get(1).asInt());     //delete the received element from FIFO
        }
            
        //increment and push back the natural N
        intCNT++;
        Fifo.enqueue(intCNT);
            
        //make a command and send it
        botCommand.addInt(COLLATZ_VOCAB_ITEM);
        botCommand.addInt(intCNT);
        botCommand.addInt(Fifo.head_value()-1);
        return true;
    }
    else{
        cout << "receiced another message" << endl;
        return false;
    }
}
    
// Configure function. Receive a previously initialized
// resource finder object. Use it to configure your module.
// If you are migrating from the old module, this is the function
// equivalent to the "open" method.
bool MyModule::configure(yarp::os::ResourceFinder &rf)
{
    intCNT=1;     //set initial value of the counter   
    // optional, attach a port to the module
    // so that messages received from the port are redirected
    // to the respond method
    handlerPort.open("/server");
    attach(handlerPort);
    return true;
}

// Close function, to perform cleanup.
bool MyModule::close()
{
    // optional, close port explicitly
    cout << "Calling close function\n";
    handlerPort.close();
    return true;
}