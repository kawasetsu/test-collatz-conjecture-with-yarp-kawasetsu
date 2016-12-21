#include <iostream>
#include <yarp/os/all.h>

using namespace std;
using namespace yarp::os;

#define FIFO_SIZE 10

class FIFO{
public:
	int intHead;
	int intQueLen;
	int intFIFO[FIFO_SIZE];
public:
	FIFO();
	int enqueue(int intEnqData);
	int dequeue();
	int head_value();
	void delete_element(int intNatural);
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
		if(intHead+1 < FIFO_SIZE){
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

void FIFO::delete_element(int intNatural)
{
	int intLen = intQueLen;
	for(int i=0; i<intLen; i++){
		if(intNatural == intFIFO[intHead]){
			dequeue();
		}
		else{
			enqueue(intFIFO[intHead]);
			dequeue();
		}
	}
}

/*class Thread1 : public RateThread{
public:
	Thread1(int r):RateThread(r){}
	virtual bool threadInit()
	{
		cout << "Starting thread" << endl;
		return true;
	}

	virtual void afterStart(bool s)
	{
		if(s){
			cout << "Thread started successfully" << endl;
		}else{
			cout << "Thread didn't start" << endl;
		}
	}

	virtual void run()
	{
		FIFO Fifo;
		for(int i = 0; i < Fifo.intQueLen; i++){
			cout << "FIFO[" << i << "]=" << Fifo.intFIFO[(Fifo.intHead+i)%FIFO_SIZE] << endl; 
		}
	}

	virtual void threadRelease()
	{
		cout << "Goodbye from thread" << endl;
	}
};*/


int main(int argc, char * argv[])
{
    Network yarp;	//initialize yarp
    RpcServer portP;
    portP.open("/server");
    FIFO Fifo;
    
    /*Thread1 t1(1000);	//run every 1s
    bool ok = t1.start();
    if(!ok){
    	cout << "The thread failed to initialize" << endl;
    	return 0;
    }*/

	int intCNT = 1;

	while(true){
		Bottle botRequest;
		Bottle botCommand;
		portP.read(botRequest, true);

		//cout << "Request=" << botRequest.get(1).asInt()  << endl;

		/*if(botRequest.get(1).asInt() == Fifo.head_value()){
			Fifo.dequeue();
			//cout << "dequeue" << endl;
		}
		else if(botRequest.get(1).asInt() != 0){
			Fifo.delete_element(botRequest.get(1).asInt());
		}*/

		if(botRequest.get(1).asInt() != 0){
			Fifo.delete_element(botRequest.get(1).asInt());
		}
		
		intCNT++;
		Fifo.enqueue(intCNT);
		
		botCommand.addString("vocab_item");
		botCommand.addInt(intCNT);
		botCommand.addInt(Fifo.head_value()-1);
		portP.reply(botCommand);

		//cout << Fifo.head_value() << endl;
		/*Time::delay(1);
		for(int i = 0; i < Fifo.intQueLen; i++){
			cout << "FIFO[" << i << "]=" << Fifo.intFIFO[(Fifo.intHead+i)%FIFO_SIZE] << endl; 
		}*/
	}

	return 0;
}