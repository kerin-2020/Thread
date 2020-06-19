#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <iosfwd>
using namespace std;



class LogFile
{
	std::mutex _mu;
	std::mutex _mu_open;
	std::once_flag _flag;
	std::ofstream _f;
public:
	LogFile()
	{
		//_f.open("log.txt");//need con to close file
	}
	void shared_print(string id, int value)
	{
		//std::unique_lock<mutex> locker2(_mu_open);
		//if (!_f.is_open())
		//{
		//	//ֻ��Ҫ��һ�Σ��˷Ѽ������Դ���������mutex�Ļ���
		//	_f.open("log.txt");
		//}

		//file will be opened once by one thread;
		std::call_once(_flag, [&]() {_f.open("log.txt"); });

		//std::lock_guard<std::mutex> locker(_mu);
		//another way
		std::unique_lock<mutex> locker(_mu, std::defer_lock);//defer_lock ���Ա༭locker����ʱ��û����
		//do sth else
		//locker.lock();
		_f << "From " << id << ": " << value << std::endl;
		//locker.unlock();

		//locker.lock();//AGAIN
		//unique lock can move to another unique lock;
		//std::unique_lock<mutex> locker2 = std::move(locker);// a little bit of heavier than lock guard
	}
};