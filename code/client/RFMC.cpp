#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "RFMC.hpp"

using namespace std;
using namespace yarp::os;

double MyModule::getPeriod()
{
    return 0.01;     // module periodicity (seconds), called implicitly by the module.
}

bool MyModule::updateModule()
{
    Bottle botRequest;
    Bottle botResponse;

    //make request and send it
    botRequest.addInt(COLLATZ_VOCAB_REQ_ITEM);
    botRequest.addInt(intN);
    bool result = handlerPort.write(botRequest, botResponse);   //request and wait response

    //cout << "intN=" << intN << endl;
    if(result){
        if(botResponse.get(0).asInt() == COLLATZ_VOCAB_ITEM){       //check the identifier of the received message
            intN = botResponse.get(1).asInt();
            intTh = botResponse.get(2).asInt();
            cout << "input number:" << intN << ", input threshold:" << intTh << endl;

            //calculate collaz conjecture
            int intTempN = intN;
            while(intTempN > intTh){    //calculation result becomes below threshold
                if(intTempN % 2 == 0){
                    intTempN = intTempN / 2;
                }
                else{
                    intTempN = intTempN * 3 + 1;
                }
            }
            //cout << "finished" << endl;
        }else{
            cout << "receiced another message" << endl;
        }
        return true;
    }
    else{
        return false;
    }
}

// Message handler
bool MyModule::respond(const Bottle& botResponse, Bottle& botCommand)
{    
    return true;
}
    
// Configure function. Receive a previously initialized
// resource finder object. Use it to configure your module.
// If you are migrating from the old module, this is the function
// equivalent to the "open" method.
bool MyModule::configure(yarp::os::ResourceFinder &rf)
{
    intN = 0;
    intTh = 0;
    handlerPort.open(rf.find("name").asString());

    cout << "Trying to connect to server" << endl;
    yarp.connect(rf.find("name").asString() , "/server");

    if(handlerPort.getOutputCount() == 0){
        cout << "cannot connect to /server" << endl;
        return false;
    }

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