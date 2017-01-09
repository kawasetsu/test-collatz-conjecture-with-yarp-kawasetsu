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
   		//if FIFO has elements, show them
		if (vectFIFO.size() > 0){
			cout << "[";
			for (int i = 0; i < vectFIFO.size(); i++){
				cout << vectFIFO(i) << ",";
			}
			cout << "]" << endl;
		}
	}

	virtual void threadRelease()
	{
		cout << "Goodbye from thread" << endl;
	}
};