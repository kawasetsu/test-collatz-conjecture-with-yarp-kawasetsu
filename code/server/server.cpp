#include <iostream>
#include <yarp/os/all.h>

using namespace std;
using namespace yarp::os;

#define FIFO_SIZE 10

class FIFO{
private:
	int intHead;
	int intQueLen;
	int intFIFO[FIFO_SIZE];
public:
	FIFO();
	int enqueue(int intEnqData);
	int dequeue();
	int head_value();
};

FIFO::FIFO()
{
	intHead = 0;
	intQueLen = 0;
	for(int i=0;i<FIFO_SIZE;i++){
		intFIFO[i]=0;
	}
}

int FIFO::enqueue(int intEnqData)
{
	if(intQueLen < FIFO_SIZE){
		intFIFO[(intHead + intQueLen) % FIFO_SIZE] = intEnqData;
		intQueLen++;
		return true;
	}else{
		return false;
	}
}

int FIFO::dequeue()
{
	if(intQueLen>0){
		intQueLen--;
		if(intHead+1 < FIFO_SIZE - 1){
			intHead++;
		}else{
			intHead = 0;
		}
		return true;
	}else{
		return false;
	}
}

int FIFO::head_value()
{
	return (intFIFO[intHead]);
}

int main(int argc, char * argv[])
{
    Network yarp;	//initialize yarp
    RpcServer portP;
    portP.open("/server");
    
    FIFO Fifo;

	int intCNT = 1;

	while(true){
		Bottle botRequest;
		Bottle botCommand;
		portP.read(botRequest, true);

		//cout << "Request=" << botRequest.get(1).asInt()  << endl;

		if(botRequest.get(1).asInt() == Fifo.head_value()){
			Fifo.dequeue();
			//cout << "dequeue" << endl;
		}
		
		intCNT++;
		Fifo.enqueue(intCNT);
		
		botCommand.addString("vocab_item");
		botCommand.addInt(intCNT);
		botCommand.addInt(Fifo.head_value()-1);
		portP.reply(botCommand);

		//cout << Fifo.head_value() << endl;
		Time::delay(1);
	}

	return 0;
}