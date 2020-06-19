#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>//互斥
#include <iosfwd>
using namespace std;



class LogFile
{
	std::mutex m_mutex;
	std::ofstream f;
public:
	LogFile()
	{
		f.open("log.txt");//need con to close file
	}
	void shared_print(string id, int value)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		//mu.lock();//别的线程不许工作
		f << "From " << id << ": "<< value << std::endl;//如果此句报错，则永远锁了线程
		//mu.unlock();
	}
	
	/*ofstream& getStream()
	{
		return f;
	}*/
	// Never go this way because user has oppertunity to access f without mutex
	//Never pass f as an argument to user
	void proF(void fun(ofstream&))
	{
		fun(f);
	}//nightmare
};

class stack
{
	int* __data;
	std::mutex _mu;
public:
	void pop();
	int& top();
};

//void function_1(stack& st)
//{
//	/*for (int i = 0; i > -100; i--)
//		log.shared_print(string("from t1 says: "), i);*/
//	int v = st.top();
//	st.pop();
//	process(v);
//}//safe?

void function_1(stack& st)
{
	int v = st.pop();
	process(v);
}
//data race 数据竞争：
//use mutex to syncrhonize data access
//never leak a handle of data to outside
//design interface appropriately

/*
the operation of top and pop should be combinded into one func
*/

int main()
{
	LogFile log;
	std::thread t1(function_1, std::ref(log));//t1 running
	//whenever a statement can be interpreted as a function declaration, it will be treated as a function declaration

	for (int i = 0; i < 100; i++)
		log.shared_print(string("From main: "), i);

	


	t1.join(); // WAIT to finish
	
	return 0;
}