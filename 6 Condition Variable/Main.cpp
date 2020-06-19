#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

using namespace std;

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;//have to use unique lock

void function1()
{
	int count = 0;
	while (count > 0)
	{
		std::unique_lock<mutex> locker(mu);
		q.push_front(count);
		locker.unlock();
		cond.notify_one();//Notify one waiting thread, if there is one.{ONE}
		//cond.notify_all();
		std::this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

void function2()
{
	//consumer
	int data = 0;
	while (data != 1)
	{
		std::unique_lock<mutex> locker(mu);
		//if (!q.empty())
		//{
		//	data = q.back();
		//	q.pop_back();
		//	locker.unlock();
		//	std::cout << "t2 got a value from t1" << data << std::endl;
		//}
		//else
		//{
		//	locker.unlock();//keep waiting, inefficient,but Timing is a problem-->condition variable cond
		//}
		cond.wait(locker, []() { return !q.empty(); });/*线程sleep的时候不应该lock，在此句之前，应该先unlock mutex*/
		//spurious wake 自己唤醒自己，用一个lambda来抑制自身的唤醒

		data = q.back();
		q.pop_back();
		locker.unlock();
		std::cout << "t2 got a value from t1" << data << std::endl;
	}
}

int main()
{
	std::thread t1(function1);
	std::thread t2(function2);
	t1.join();
	t2.join();

	return 0;
}