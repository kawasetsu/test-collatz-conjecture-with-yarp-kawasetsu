#include <iostream>
#include <yarp/os/all.h>
#include "../common/vocabs.hpp"
#include "Client.hpp"

using namespace std;
using namespace yarp::os;

bool Client::configure(ResourceFinder &rf)
{
    intN = 0;
    intTh = 0;
    handlerPort.open(rf.find("name").asString());

    cout << "Trying to connect to server" << endl;
    yarp.connect(rf.find("name").asString(), "/server");

    if(handlerPort.getOutputCount() == 0){
        cout << "cannot connect to /server" << endl;
        return false;
    }

    return true;
}

double Client::getPeriod()
{
    // call update module every 0.2 sec.
    return 0.2;
}

bool Client::updateModule()
{
    Bottle botRequest;
    Bottle botResponse;

    //make request
    botRequest.addInt(COLLATZ_VOCAB_REQ_ITEM);
    botRequest.addInt(intN);
    //send the request and wait a response from server
    bool result = handlerPort.write(botRequest, botResponse);

    if(result){
        //check the header of received message
        if(botResponse.get(0).asInt() == COLLATZ_VOCAB_ITEM){
            intN = botResponse.get(1).asInt();
            intTh = botResponse.get(2).asInt();
            cout << "input number:" << intN << ", input threshold:" << intTh << endl;

            //calculate number sequence according to the collatz conjecture
            int intTempN = intN;
            //wait calculation result becomes below threshold
            while(intTempN > intTh){
                if(intTempN % 2 == 0){
                    intTempN = intTempN / 2;
                }
                else{
                    intTempN = intTempN * 3 + 1;
                }
            }
        }else{
            cout << "receiced another message" << endl;
        }
        return true;
    }
    else{
        return false;
    }
}

// Close function, to perform cleanup.
bool Client::close()
{
    cout << "Calling close function" << endl;
    handlerPort.close();
    return true;
}