#ifndef __FIFO_HPP_INCLUDED__
#define __FIFO_HPP_INCLUDED__

#include <vector>
#include <yarp/os/all.h>
using namespace std;
using namespace yarp::os;

class FIFO{
	Semaphore sema;
	vector<int> vectFIFO;
public:
	FIFO();
	void enqueue(int intEnqData);
	bool dequeue();
	int head_value();
	void delete_element(int intNatural);
	void show();
};

#endif