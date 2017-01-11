#include <iostream>
#include <vector>
#include <yarp/os/all.h>
#include "FIFO.hpp"

using namespace std;
using namespace yarp::os;

FIFO::FIFO()	//constructor
{
}

void FIFO::enqueue(int intEnqData)	//enqueue the argument "intEnqData"
{
	sema.wait();
	vectFIFO.push_back(intEnqData);
	sema.post();
}

bool FIFO::dequeue()		//dequeue the first element of FIFO
{
	if(!vectFIFO.empty()){
		sema.wait();
		vectFIFO.erase(vectFIFO.begin());
		sema.post();
		return true;
	}else{
		return false;
	}
}

int FIFO::head_value()	//return the value of the first element of FIFO
{
	return (vectFIFO.front());
}

void FIFO::delete_element(int intNatural)	//delete the element of FIFO corespoinding to the argument "intNatural"
{
	if(!vectFIFO.empty()){
		sema.wait();
		for(int i = 0; i < vectFIFO.size(); i++){
			if(intNatural == vectFIFO[i]){
				vectFIFO.erase(vectFIFO.begin()+i);
				break;
			}
		}
		sema.post();
	}
}

void FIFO::show()
{
	if(!vectFIFO.empty()){
		cout << "[" ;
		sema.wait();
		for(int i = 0; i < vectFIFO.size(); i++){
			cout << vectFIFO[i] << " " ;
		}
		sema.post();
		cout << "]" << endl;
	}
}