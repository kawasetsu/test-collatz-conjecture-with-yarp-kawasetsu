#include "RFMC.hpp"
    
// Message handler. Just echo all received messages.
bool respond(const Bottle& botResponse, Bottle& botCommand)
{
    if(botResponse.get(0) == COLLATZ_VOCAB_REQ_ITEM){           //check the header of received messages
        if(botRequest.get(1).asInt() != 0){                     //first connection from client sends 0, so skip
            Fifo.delete_element(botRequest.get(1).asInt());     //delete the received element from FIFO
        }
            
        //increment and push back the natural N
        intCNT++;
        Fifo.enqueue(intCNT);

        if (Sema.check() == true){
            Sema.post();
        }
            
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


    //make request and send it
        botRequest.addInt(COLLATZ_VOCAB_REQ_ITEM);
        botRequest.addInt(intN);
        portP.write(botRequest, botResponse);   //request and wait response

        //cout << "intN=" << intN << endl;

        if(botResponse.get(0) == COLLATZ_VOCAB_ITEM){       //check the identifier of the received message
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
            cout << "finished" << endl;

        }else{
            cout << "receiced another message" << endl;
        }
}
    
// Configure function. Receive a previously initialized
// resource finder object. Use it to configure your module.
// If you are migrating from the old module, this is the function
// equivalent to the "open" method.
bool configure(yarp::os::ResourceFinder &rf)
{
    count=0;
    // optional, attach a port to the module
    // so that messages received from the port are redirected
    // to the respond method
    handlerPort.open("/myModule");
    attach(handlerPort);
    return true;
}

// Interrupt function.
bool interruptModule()
{
    cout << "Interrupting your module, for port cleanup" << endl;
    return true;
}

// Close function, to perform cleanup.
bool close()
{
    // optional, close port explicitly
    cout << "Calling close function\n";
    handlerPort.close();
    return true;
}