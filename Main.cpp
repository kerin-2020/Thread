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
	//parameter to a thread is always passed by value, ���һ��Ҫ�����õĻ���ʹ�� std::ref()
	std::thread t1((Fctor()), std::move(s));
	//std::cout << t1.get_id() << std::endl;
	//move the s fromn main thread to the child thread, safe and efficient
	//thread object can only be moved  t2=t1 NO!
	//std::thread t2 = std::move(t1);//t1 is empty

	//��Ҫ�������ڼ�����߳��������س�
	//std::thread::hardware_concurrency();

	//try
	//{
	//	//for (int i = 0; i < 100; i++)
	//		std::cout << "From main: " << s << std::endl;// t1 �ڴ˾�ִ����ǰ����
	//}
	//catch (...)
	//{
	//	t1.join();
	//	throw;
	//}
	
	

	t1.join(); // WAIT to finish
	std::cout << "From main: " << s << std::endl;
	//t1.detach();//���̻��գ����߳�̫���޷���ʾ���������

	/*if (t1.joinable())
		t1.join();*/

	return 0;
}