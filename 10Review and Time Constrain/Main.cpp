#include <iostream>
#include <thread>
#include <future>//packaged_task
#include <string>
#include <deque>

using namespace std;


int factorial(int N)
{
	int res = 1;

	for (int i = N; i > 1; i--)
		res *= i;

	std::cout << "Result is :" << res << endl;
	return res;
}

int main()
{
	/*thread*/
	std::thread t1(factorial, 6);
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::microseconds(4);
	std::this_thread::sleep_until(tp);

	/*mutex*/
	std::mutex mu;
	std::lock_guard<mutex> locker(mu);
	std::unique_lock<mutex> ulocker(mu);
	ulocker.try_lock();
	ulocker.try_lock_for(std::chrono::nanoseconds(500));
	ulocker.try_lock_until(tp);

	/*condition_variable*/
	std::condition_variable cond;
	cond.wait_for(ulocker, std::chrono::microseconds(2));
	cond.wait_until(ulocker, tp);

	/*future & promise*/
	std::promise<int> p;
	std::future<int> f = p.get_future();
	f.get();
	f.wait();
	f.wait_for(std::chrono::microseconds(3));
	f.wait_until(tp);

	/*async*/
	std::future<int> fu = std::async(factorial, 6);

	/*packaged_task: A class template that can be parameterized the way the funciton signature of the
	(参数化类签名方式的类模板)
	tast we are going to create in this case: factorial
	*/
	std::packaged_task<int(int)> t(factorial);
	std::future<int> fu2 = t.get_future();
	t(6);

}