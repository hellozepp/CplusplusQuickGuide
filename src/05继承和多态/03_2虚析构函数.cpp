/*** 
 * @Author: helintongh
 * @Date: 2020-09-05 17:41:53
 * @LastEditTime: 2020-09-05 17:55:18
 * @LastEditors: helintongh
 * @Description: 
 * @FilePath: /hlt/继承与多态/虚析构函数.cpp
 */

/*
问题一：哪些函数不能实现成虚函数？

虚函数依赖：
1.虚函数能产生地址，存储在vftable当中
2.对象必须存在(vfptr -> vftable -> 虚函数地址)

构造函数
1.virtual+构造函数 不行! 
2.构造函数中（调用的任何函数，都是静态绑定的）调用虚函数，也不会发生动态绑定

+++
注意：Java方法的晚绑定：没有被final关键词修饰的方法都是在运行时才被确定的，而不是根据代码中引用的类型静态确定的。
如：在Basic的构造函数中，调用了add方法，这个调用实际是省略了this关键词的，所以实际上应该是：
Basic ref = this;
ref.add('a');
即使ref引用的类型是Basic，但它的add方法未必就是Basic类中定义的add方法，也有可能是它子类定义的add方法（实际上这就是多态）。
所以，c++的构造函数中调用虚函数不会发生多态，而java的构造函数中调用虚函数会发生多态。
+++

派生类对象构造过程  1.先调用的是基类的构造函数 2.才调用派生类的构造函数

static静态成员方法(不依赖对象) 不行! virtual + static

问题二：虚析构函数 析构函数调用的时候，对象是存在的！
什么时候把基类的析构函数必须实现成虚函数？
基类的指针（引用）指向堆上new出来的派生类对象的时候， delete pb(基类的指针)
，它调用析构函数的时候，必须发生动态绑定，否则会导致派生类的析构函数无法调用
*/
#include <iostream>
using namespace std;

class Base
{
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }
	// 虚析构函数
	virtual ~Base() { cout << "~Base()" << endl; }
	virtual void show() { cout << "call Base::show()" << endl; }
protected:
	int ma;
}; // &Base::~Base   &Base::show
class Derive : public Base // &Derive::~Derive   &Base::show 
{
public:
	Derive(int data)
		:Base(data), mb(data), ptr(new int(data))
	{
		cout << "Derive()" << endl;
	}
	// 基类的析构函数是virtual虚函数，那么派生类的析构函数自动成为虚函数
	~Derive()
	{
		delete ptr;
		cout << "~Derive()" << endl;
	}
private:
	int mb;
	int *ptr;
};
int main()
{
    // Driver d(10); // 构造栈对象，不用自己delete
	Base *pb = new Derive(10);
	pb->show(); //  如果show是虚函数，动态绑定 pb Base*  *pb Derive
	delete pb; // 如果没有虚析构函数,派生类的析构函数没有被调用到！！因为 pb 是基类指针，调用析构是调用的base的析构函数
   
    /*
	pb->Base  Base::~Base (普通函数)对于析构函数的调用，就是静态绑定了
	call Base::~Base

	pb->Base Base::~Base (虚函数)对于析构函数的调用，就是动态绑定了
	pb -> Derive Derive vftable &Derive::~Derive
	*/
	return 0;
}
