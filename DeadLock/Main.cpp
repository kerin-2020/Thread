#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>//互斥
#include <iosfwd>
using namespace std;



class LogFile
{
	std::mutex _mu;
	std::mutex _mu2;
	std::ofstream _f;
public:
	LogFile()
	{
		_f.open("log.txt");//need con to close file
	}
	void shared_print(string id, int value)
	{
		//Advanced
		std::lock(_mu, _mu2);
		std::lock_guard<std::mutex> locker(_mu, std::adopt_lock);
		std::lock_guard<std::mutex> locker2(_mu2, std::adopt_lock);
		std::cout << "From " << id << ": " << value << std::endl;
	}
	void shared_print2(string id, int value)
	{
		std::lock_guard<std::mutex> locker(_mu );
		std::lock_guard<std::mutex> locker2(_mu2);
		std::cout << "From " << id << ": " << value << std::endl; 
	}
};

void function_1(LogFile& log)
{
	for (int i = 0; i > -100; i--)
	{
		log.shared_print("t1", i);
	}
}


/*
//死锁现象：t1锁了mu在准备锁mu2时
main锁了mu2，等待t1 unlock mu
避免：锁mutex顺序保持一致

思考：是否需要真的需要双线程lock

avoid:
1. prefer lock one single mutex
2. avoid locking a mutex and then calling a user provided function
3. Use std::lock() to lock more than one mutex
4. lock the mutex in same order

- Fine-grained lock: protects small amount of data
- Coarse-grained lock: protects big amount of data
*/

int main()
{
	LogFile log;
	std::thread t1(function_1, std::ref(log));

	for (int i = 0; i < 100; i++)
		log.shared_print2(string("From main: "), i);

	t1.join(); // WAIT to finish

	return 0;
}