#include <iostream>
#include <vector>
#include <yarp/os/all.h>
#include "FIFO.hpp"

using namespace std;
using namespace yarp::os;

//constructor
FIFO::FIFO()
{
}

//enqueue the argument "intEnqData"
void FIFO::enqueue(int intEnqData)
{
	sema.wait();
	vectFIFO.push_back(intEnqData);
	sema.post();
}

//return the value of the first element of FIFO
int FIFO::head_value()
{
	int intFront;
	
	sema.wait();
	intFront = vectFIFO.front();
	sema.post();

	return (intFront);
}

//delete the element of FIFO corespoinding to the argument "intNatural"
void FIFO::delete_element(int intNatural)
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
		cout << "pending numbers: " ;
		sema.wait();
		for(int i = 0; i < vectFIFO.size(); i++){
			cout << vectFIFO[i] << " " ;
		}
		sema.post();
		cout << endl;
	}
}