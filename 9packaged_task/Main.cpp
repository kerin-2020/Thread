#include <iostream>
#include <thread>
#include <future>//packaged_task
#include <string>
#include <deque>

using namespace std;

/* packeaged_task

packaged_task provides a way to implement a task pool.
It can conveniently convey the returned value from a task to a different thread.
*/
int factorial(int N)
{
	int res = 1;

	for (int i = N; i > 1; i--)
		res *= i;

	std::cout << "Result is :" << res << endl;
	return res;
}

std::deque<std::packaged_task<int()> > task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1()
{
	std::packaged_task<int()> t;
	{
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, []() {return !task_q.empty(); });
		t = std::move(task_q.front()); //�������д�����һ�������߳�t1���ŵ�task_q�У�����thread_1��ȡ��ִ��
		task_q.pop_front();
		//����Ŀ3�����ᵽ��stack�����ݽṹ����Ѳ�����ȡֵ�ŵ�һ��mutex�У��̲߳Ų��ᶪʧ����
	}
	t();
}

int mian()
{
	std::thread t1(thread_1);
	std::packaged_task<int()> t(std::bind(factorial, 6));//signature,can not take parameter anymore, because it is binded
	std::future<int> fu = t.get_future();
	{
		std::lock_guard<std::mutex> locker(mu);
		task_q.push_back(std::move(t));
	}
	cond.notify_one();
	/*t = std::move(task_q.front());*/
	
	//t(6);// in a different context return void
	//int x = t.get_future().get();

	//packeged_task can link a callable object to a future and that is very important in a thread environment
	/*auto t = std::bind(factorial, 6);
	;*/


	std::cout << fu.get() << std::endl;
	t1.join();

	return 0;
}

/*
3 ways to get a future:
	- promise::get_future()
	- packaged_task::get_future()
	- async() return a future
*/