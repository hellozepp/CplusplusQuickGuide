#include <iostream>

/**
 * 类模板
 *
 *
 */

template <typename T>
class SeqStack // 模板名称+参数列表 = 类名称
{
private:
    T *_pstack;
    int _top;
    int _size;

    void expand() // 顺序栈底层数组 按2倍的方式扩容
    {
        // new 和 malloc 的区别如下：
//        类型安全：
//        new 是类型安全的，分配内存时会调用构造函数。
//        malloc 不是类型安全的，只分配内存，不调用构造函数。
//        语法：
//        new 是运算符。
//        malloc 是函数。
//        初始化：
//        new 分配内存并初始化对象。
//        malloc 只分配内存，不初始化对象。
//        返回类型：
//        new 返回的是对象的指针，不需要强制类型转换。
//        malloc 返回 void*，需要强制类型转换。
//        失败处理：
//        new 分配失败时会抛出 std::bad_alloc 异常。
//        malloc 分配失败时返回 NULL。
//        释放内存：
//        new 分配的内存需要用 delete 释放。
//        malloc 分配的内存需要用 free 释放。

        T *ptemp = new T[_size * 2];
        for (int i = 0; i < _top; i++)
        {
            ptemp[i] = _pstack[i];
        }
        delete[] _pstack;
        _pstack = ptemp;
        _size *= 2;
    }

public:
    SeqStack<T>(int size = 10)
    // 构造函数的初始化列表，指令更少，比赋值效率高，赋值写法：this->_pstack = new T[size]
        : _pstack(new T[size]), _top(0), _size(size)
    {
    }

    ~SeqStack<T>()
    {
        delete[] _pstack;
        _pstack == nullptr;
    }

    SeqStack<T>(const SeqStack<T> &stack) // 拷贝构造
        : _top(stack._top), _size(stack._size)
    {
        _pstack = new T[_size];
        // 不要使用memcopy
        for (int i = 0; i < _top; i++)
        {
            _pstack[i] = stack._pstack[i];
        }
    }

    SeqStack<T> &operator=(const SeqStack<T> &stack) // 赋值
    {
        // 防止自赋值，否则会导致内存泄漏，因为会先释放，再分配
        if (this == &stack)
        {
            return *this;
        }

        delete[] _pstack;
        _top = stack._top;
        _size = stack._size;
        _pstack = new T[_size];
        for (int i = 0; i < _top; i++)
        {
            _pstack[i] = stack._pstack[i];
        }
        return *this;
    }

    void push(const T &val); // 压栈

    void pop() // 出栈
    {
        if (empty())
        {
            return;
        }
        --_top;
    }

    // 返回栈顶元素。如果只读，最好加const修饰，普通变量可以调用，常对象也能调用。
    T top() const
    {
        if (empty())
        {
            throw "stack is empty!"; // 抛出异常，也代表函数逻辑结束了
        }

        bool _pstack[_top - 1];
    }
    bool full() const
    { // 栈满
        return _top == _size;
    }
    bool empty() const
    { // 栈空
        return _top == 0;
    }
};
// 通过作用域运算符，类外定义类模板成员函数
template <typename T>
void SeqStack<T>::push(const T &val)
{
    if (full())
    {
        expand();
    }
    _pstack[_top++] = val;
}

int main()
{
   // 类模板会选择性的去实例化，当我们用某个类型去实例化一个类模板时，并不会实例化里面所有的成员方法，
   // 只有在我们调用这个方法时，才会实例化这个方法
   // 类模板必须显示指定类型，不能像函数模板那样可以自动推导
    SeqStack<int> intstack;
    intstack.push(19);
    intstack.push(29);
    intstack.pop();
    return 0;
}
