
#include <iostream>
#include <cstring>
using namespace std;

/*
C++ 函数模板
模板的意义: 对类型也可以进行参数化了
int sum(int a, int b) {return a+b;}

函数模板 <= 是不进行编译的，因为类型还不知道
模板的实例化 <= 函数调用点进行实例化
模板的函数 <= 才是要被编译器所编译的

模板类型参数 typename/class 可定义多个参数
模板非类型参数

模板的实参推演 => 可以根据用户传入的实参的类型，来推导出模板类型参数的具体类型 e.g: compare(30, 20);

模板的特例化(专用化) 特殊（不是编译器提供的，而是用户提供的）的实例化
函数模板、模板的特例化、非模板函数的重载关系

注:
模板代码是不能在一个文件中定义，在另外一个文件中使用的
模板代码调用之前，一定要看到模板定义的地方，这样的话，模板
才能够进行正常的实例化，产生能够被编译器编译的代码

所以，模板代码都是放在头文件当中的，然后在源文件当中直接
进行#include包含
*/
template<typename T> // 定义一个模板参数列表，类型参数可以用class，也可以用typename 关键字
bool compare(T a, T b) // compare是一个函数模板（不编译，实例化才编译）
{
    cout << "templata compare" << endl;
    return a > b;
}

// 告诉编译器，需要进行指定类型的模板实例化(如果模板定义和实现在不同的文件中，可以强行实例化，实际上模板代码写在头文件更好)
template bool compare<int>(int, int);
template bool compare<double>(double, double);

/*
在函数调用点,编译器用用户指定的类型，从原模版实例化一份函数代码出来
模板函数
bool compare<int>(int a, int b) 
{
	return a > b;
}
bool compare<double>(double a, double b)
{
	return a > b;
}
*/

// 针对compare函数模版，提供const char*类型的特例化版本（字符串比较特殊，stl库也是提供了字符串的特例化版本）
template<>
bool compare<const char*>(const char *a, const char *b)
{
    cout << "compare<const char *>" << endl;
    return strcmp(a, b) > 0;
}
// 非模板函数--普通函数，没有定义模板参数列表，所以可以共存。c语言不能定义函数重载，c++可以
bool compare(const char *a, const char *b)
{
    cout << "normal compare" << endl;
    return strcmp(a, b) > 0;
}
int main()
{
    // （在函数调用点,编译器用用户指定的类型,从原模板实例化一份函数代码出来->函数模板,对于编译器来说，每次调用实例化模板，同一个类型只创建一次，对用户来说减少了工作量）
    compare<int>(10, 20); // 模板+参数列表才叫做函数名
    // double 被转换成了int类型，会有个警告
    compare<int>(10, 20.2);
    compare<double>(10.5, 20.5);

    // 函数模板实参的推演
    compare(20, 30); //也可进行函数调用 <=通过模板的实参推演
    //compare(30, 40.5); // 出问题,推导不出具体类型,解决方法a和b定义不同的模板类型
    compare<int>(30, 40.5); // 这样就可
    // 函数模板实参的推演 T const char*
	// 对于某些类型来说，依赖编译器默认实例化的模板代码，代码处理逻辑是有错误的
	// 编译器优先把compare处理成函数名字，没有的化，才去找compare模板
    compare("aaa", "bbb"); // 调用的是普通函数
    compare<const char*>("aaa", "bbb");
    compare<const char *>("abv", "abc"); // 此时使用特例化版本

    int arr[] = {12, 5, 7, 89, 32, 21, 36};
    // 常量表达式在编译的时候就被执行过了
    const int size = sizeof(arr) / sizeof(arr[0]);
    sort<int, size>(arr);



    return 0;
}
