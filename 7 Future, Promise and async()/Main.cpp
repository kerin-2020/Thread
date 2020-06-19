#include <iostream>
#include <thread>
#include <future>

using namespace std;

//std::mutex mu;
//std::condition_variable cond;

//异步

int factorial(std::shared_future<int> f)
{
	int res = 1;

	int N = f.get();//这样N可以从子线程传到主线程中,如果主线程没有set, create std::future_errc::broken_promise 
	for(int i = N; i > 1; i--)
	{
		res *= i;
	}

	std::cout << "Result is :" << res << endl;
	return res;
}

int mian()
{
	int x;
	/*std::thread t1(factorial, 4,std::ref(x));
	t1.join();*/

	//不仅可以实现父线程get子线程的数，也可以反过来
	//我们也可以实现子线程到父线程，不是在创建的时候，而是在中间的某个时候:promise
	std::promise<int> p;
	// can only be moved std::move()
	std::future<int> f = p.get_future();

	//std::future<int> fu = std::async(factorial, 4);//get sth in the future//①
	//std::future<int> fu = std::async(std::launch::deferred, factorial, 4);//在get后生成线程
	//std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));//生成另一个线程
	//std::future<int> fu = std::async(std::launch::async | std::launch::deferred, factorial, std::ref(f));//调用哪一个是由implementation决定
	//和①是相同效果

	std::shared_future<int> sf = f.share();//can be copied

	std::future<int> fu = std::async(std::launch::async, factorial, sf);
	std::future<int> fu2 = std::async(std::launch::async, factorial, sf);
	std::future<int> fu3 = std::async(std::launch::async, factorial, sf);
	//...10 threads
	//这样可以实现一种广播通信模型

	//std::this_thread::sleep_for(std::chrono::milliseconds(20));
	p.set_value(4);
	//x = fu.get();//x获得子线程的返回值,只能get一次
	//std::cout << "Get from child: " << x << std::endl;

	//p.set_exception(std::make_exception_ptr(std::runtime_error("To Err is human")));

	return 0;
}