#include <iostream>
#include <thread>
using namespace std;

void function_1()
{
	std::cout << "Beauty is only skin-deep" << std::endl;
}

class Fctor {
public:
	void operator()(string& msg)
	{
		//for (int i = 0; i > -100; i--)
		//{
			std::cout << "from t1 says: " << msg << std::endl;
			msg = "Trust is the mother if deceit.";
		//}
	}
};

int main()
{
	/*Fctor fct;
	std::thread t1(fct);*/
	//std::thread t1(function_1);//t1 running
	//whenever a statement can be interpreted as a function declaration, it will be treated as a function declaration

	string s = "where there is no trust, there is no love";
	
	/*std::thread t1((Fctor()), std::ref(s));*/
	//parameter to a thread is always passed by value, 如果一定要传引用的话，使用 std::ref()
	std::thread t1((Fctor()), std::move(s));
	//std::cout << t1.get_id() << std::endl;
	//move the s fromn main thread to the child thread, safe and efficient
	//thread object can only be moved  t2=t1 NO!
	//std::thread t2 = std::move(t1);//t1 is empty

	//不要创建多于计算机线程容量的县城
	//std::thread::hardware_concurrency();

	//try
	//{
	//	//for (int i = 0; i < 100; i++)
	//		std::cout << "From main: " << s << std::endl;// t1 在此句执行完前结束
	//}
	//catch (...)
	//{
	//	t1.join();
	//	throw;
	//}
	
	

	t1.join(); // WAIT to finish
	std::cout << "From main: " << s << std::endl;
	//t1.detach();//进程回收，主线程太快无法显示，并发编程

	/*if (t1.joinable())
		t1.join();*/

	return 0;
}