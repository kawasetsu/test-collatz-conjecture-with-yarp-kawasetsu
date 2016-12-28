#include <iostream>
#include <yarp/os/all.h>

using namespace std;
using namespace yarp::os;

#define FIFO_SIZE 10

volatile int intFIFOtmp[FIFO_SIZE];
volatile int intFIFOnum;

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
	void delete_element(int intNatural);
	void FIFO_copy();
};

FIFO::FIFO()	//constructor
{
	intHead = 0;
	intQueLen = 0;
	for(int i=0;i<FIFO_SIZE;i++){
		intFIFO[i]=0;
	}
}

int FIFO::enqueue(int intEnqData)	//enqueue the argument "intEnqData"
{
	if(intQueLen < FIFO_SIZE){
		intFIFO[(intHead + intQueLen) % FIFO_SIZE] = intEnqData;
		intQueLen++;
		FIFO_copy();	//copy the content of FIFO to global variable for monitoring
		return true;
	}else{
		return false;
	}
}

int FIFO::dequeue()		//dequeue the first element of FIFO
{
	if(intQueLen>0){
		intQueLen--;
		if(intHead+1 < FIFO_SIZE){
			intHead++;
		}else{
			intHead = 0;
		}
		FIFO_copy();	//copy the content of FIFO to global variable for monitoring
		return true;
	}else{
		return false;
	}
}

int FIFO::head_value()	//return the value of the first element of FIFO
{
	return (intFIFO[intHead]);
}

void FIFO::delete_element(int intNatural)	//delete the element of FIFO corespoinding to the argument "intNatural"
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

void FIFO::FIFO_copy()
{
	if(intQueLen>0){
		for(int i = 0; i < intQueLen; i++){
			intFIFOtmp[i] = intFIFO[i];
		}
		intFIFOnum = intQueLen;
	}
}

class Thread1 : public RateThread{
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
   		Semaphore Sema;
   		Sema.wait();	//wait update procedure in main function

   		//if FIFO has elements, show them
		if (intFIFOnum > 0){
			for (int i = 0; i < intFIFOnum; i++){
				cout << "FIFO[" << i << "]=" << intFIFOtmp[i] << ",";
			}
			cout << endl;
		}
	}

	virtual void threadRelease()
	{
		cout << "Goodbye from thread" << endl;
	}
};

int main(int argc, char * argv[])
{
    Network yarp;	//initialize yarp
    Port portP;
    portP.open("/server");
    FIFO Fifo;
    Semaphore Sema;

    //initialize grobal variables for monitoring of the content of FIFO
    intFIFOnum = 0;
    for(int i = 0; i<FIFO_SIZE; i++){
    	intFIFOtmp[i] = 0;
    }
    
    Thread1 t1(5000);	//report the content of FIFO every 5s
    bool ok = t1.start();
    if(!ok){
    	cout << "The thread failed to initialize" << endl;
    	return 0;
    }

	int intCNT = 1;		//set initial value of the counter

	while(true){
		Bottle botRequest;		//container for request from client
		Bottle botCommand;		//container for command to client
		portP.read(botRequest, true);		//wait to receive a request from client

		if(botRequest.get(1).asInt() != 0){						//first connection from client sends 0, so skip
			Fifo.delete_element(botRequest.get(1).asInt());		//delete the received element from FIFO
		}
		
		//increment and push back the natural N
		intCNT++;
		Fifo.enqueue(intCNT);

		if (Sema.check() == true){
			Sema.post();
		}
		
		//make a command and send it
		botCommand.addString("vocab_item");
		botCommand.addInt(intCNT);
		botCommand.addInt(Fifo.head_value()-1);
		portP.reply(botCommand);
	}

	return 0;
}