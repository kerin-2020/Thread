#include <iostream>
#include <thread>
#include <future>

using namespace std;

//std::mutex mu;
//std::condition_variable cond;

//�첽

int factorial(std::shared_future<int> f)
{
	int res = 1;

	int N = f.get();//����N���Դ����̴߳������߳���,������߳�û��set, create std::future_errc::broken_promise 
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

	//��������ʵ�ָ��߳�get���̵߳�����Ҳ���Է�����
	//����Ҳ����ʵ�����̵߳����̣߳������ڴ�����ʱ�򣬶������м��ĳ��ʱ��:promise
	std::promise<int> p;
	// can only be moved std::move()
	std::future<int> f = p.get_future();

	//std::future<int> fu = std::async(factorial, 4);//get sth in the future//��
	//std::future<int> fu = std::async(std::launch::deferred, factorial, 4);//��get�������߳�
	//std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));//������һ���߳�
	//std::future<int> fu = std::async(std::launch::async | std::launch::deferred, factorial, std::ref(f));//������һ������implementation����
	//�͢�����ͬЧ��

	std::shared_future<int> sf = f.share();//can be copied

	std::future<int> fu = std::async(std::launch::async, factorial, sf);
	std::future<int> fu2 = std::async(std::launch::async, factorial, sf);
	std::future<int> fu3 = std::async(std::launch::async, factorial, sf);
	//...10 threads
	//��������ʵ��һ�ֹ㲥ͨ��ģ��

	//std::this_thread::sleep_for(std::chrono::milliseconds(20));
	p.set_value(4);
	//x = fu.get();//x������̵߳ķ���ֵ,ֻ��getһ��
	//std::cout << "Get from child: " << x << std::endl;

	//p.set_exception(std::make_exception_ptr(std::runtime_error("To Err is human")));

	return 0;
}