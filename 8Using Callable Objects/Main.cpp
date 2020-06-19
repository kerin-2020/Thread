#include <iostream>
#include <thread>
#include <future>

class A
{
public:
	void f(int x, char c){}
	long g(double x) { return 0; }
	int operator()(int N) { return 0; }
};

void foo(int x){}

int main()
{
	A a;//a is a funtor·Âº¯Êý
	std::thread t1(a, 6);//copy of a() in a different thread
	std::thread t2(std::ref(a), 6);//a() in a different thread
	std::thread t2_1(std::move(a), 6);//a is no longer usable in main thread, a is now moved to a child thread
	std::thread t3(A(), 6);// temp A ???
	std::thread t4([](int x) {return x * x; }, 6);
	std::thread t5(foo, 7);
	//std::async(std::launch::async, a, 6);//create another thread
	//std::bind(a, 6);
	//std::call_once(once_flag, a, 6);

	std::thread t6(&A::f, a, 8, 'w');//copy_of_a.f(8, 'w') in a different thread;
	std::thread t7(&A::f, &a, 8, 'w');//a.f(8, 'w') in a different thread;

	//in the main function, you can call std::bind(),async,call_once

	return 0;
}