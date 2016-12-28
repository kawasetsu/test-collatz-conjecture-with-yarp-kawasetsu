#include <iostream>
#include <yarp/os/all.h>

using namespace std;
using namespace yarp::os;

int main(int argc, char * argv[])
{
	if(argc<2){
		cout << "Please supply a port name for client" << endl;
		return 1;
	}

	const char *charName = argv[1];

    Network yarp;	//initialize yarp
    Semaphore sem;
    Port portP;
    portP.open(charName);
    
	int intN = 0, intTh = 0;

	while(true){
		if(portP.getOutputCount() == 0){
			cout << "Trying to connect to server" << endl;
			yarp.connect(charName, "/server");
		}else{
		    Bottle botRequest;
		    Bottle botResponse;

		    //make request and send it
			botRequest.addString("vocab_req");
			botRequest.addInt(intN);
			portP.write(botRequest, botResponse);	//request and wait response

			//cout << "intN=" << intN << endl;

			if(botResponse.get(0) == "vocab_item"){		//check the identifier of the received message
				intN = botResponse.get(1).asInt();
				intTh = botResponse.get(2).asInt();
				cout << "input number:" << intN << ", input threshold:" << intTh << endl;

				//calculate collaz conjecture
				int intTempN = intN;
				while(intTempN > intTh){	//calculation result becomes below threshold
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
		}
	}

	return 0;
}