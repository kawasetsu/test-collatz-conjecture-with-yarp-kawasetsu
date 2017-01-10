#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "Client.hpp"

using namespace std;
using namespace yarp::os;

double Client::getPeriod()
{
    return 0.01;        // module periodicity (seconds), called implicitly by the module.
}

bool Client::updateModule()
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
bool Client::respond(const Bottle& botResponse, Bottle& botCommand)
{    
    return true;
}

bool Client::configure(yarp::os::ResourceFinder &rf)
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
bool Client::close()
{
    cout << "Calling close function\n";
    handlerPort.close();    //close port
    return true;
}