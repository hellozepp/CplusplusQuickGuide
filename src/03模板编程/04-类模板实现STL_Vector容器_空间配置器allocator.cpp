#include <iostream>

/**
 * 容器的空间配置器allocator
 *  1. 内存开辟
 *  2. 内存释放
 *  3. 对象构造
 *  4. 对象析构
 */

// 定义容器的空间配置器， 和c++标准库的allocator实现一样
// 这是一个模板类，可以为任意类型的对象开辟内存，释放内存，构造对象，析构对象
template <typename T>
struct Allocator
{
    T *allocate(size_t n) // 开辟内存
    {
        std::cout << "allocate" << std::endl;
        return static_cast<T *>(malloc(n * sizeof(T)));
    }
    void deallocate(T *p, size_t n) // 内存释放
    {
        std::cout << "deallocate" << std::endl;
        free(p);
    }
    void construct(T *p, const T &t) // 对象构造
    {
        std::cout << "construct" << std::endl;
        new (p) T(t); // 定位new表达式，调用T类型的构造函数（拷贝构造函数）。 p指向的内存位置构造一个值为t的对象，这里没有返回值，因为是构造函数的操作
    }
    void destroy(T *p) // 对象析构
    {
        std::cout << "destroy" << std::endl;
        p->~T(); //~T() 代表T类型的析构函数
    }
};

// 容器底层内存开辟、内存释放 都通过空间配置器来操作
// typename Alloc = Allocator<T> 为模板类提供Alloc类型的默认值，如果用户没有指定Alloc类型，就使用Allocator<T>类型
template <typename T, typename Alloc = Allocator<T>>
class vector
{
public:
    vector<T, Alloc>(int size = 10)
    {
        // 需要把内存开辟和对象构造分开处理，因为vector构造应该只负责内存开辟，对象构造是用户的事情
        // _first = new T[size]
        _first = _last = _end = _alloc.allocate(size);
        _end = _first + size;
        std::cout << "vector(int size = 10)" << std::endl;
    }

    ~vector<T, Alloc>()
    {
        // 析构容器有效的元素，然后释放容器的堆内存
        // 直接释放_first指针指向的数组内存，会导致有效元素的析构函数没有调用，即使数组内存释放了，但是有效元素内存没有释放
        // delete[] _first;

        // _last 指向的是有效元素的后继位置，所以需要从_first到_last进行析构操作
        for (T *p = _first; p != _last; ++p)
        {
            _alloc.destroy(p); // 调用对象的析构函数, 把_frist指针指向的数组 的有效元素进行析构操作
        }
        //  最后，释放_first指针指向的数组内存（堆上的数组内存）
        _alloc.deallocate(_first, _last - _first);

        _first = _last = _end = nullptr;
        std::cout << "~vector()" << std::endl;
    }

    // 自定义拷贝构造函数
    vector<T, Alloc>(const vector<T, Alloc> &other)
    {
        int size = other._end - other._first; // other._end - other._first 空间大小
        // _first = new T[size];
        _first = _alloc.allocate(size);

        int len = other._last - other._first; // other._last - other._first 有效元素的个数
        for (int i = 0; i < len; i++)
        {
            // _first[i] = other._first[i];
            _alloc.construct(_first + i, other._first[i]);
        }
		// 不用-1，因为_last指向的是有效元素的后继位置
        _last = _first + len;
        _end = _first + size;
        std::cout << "vector(const vector<T> &other)" << std::endl;
    }

    // 赋值重载
    vector<T, Alloc> &operator=(const vector<T> &other)
    {
        if (this != &other)
        {
            // delete[] _first;
            for (T *p = _first; p != _last; ++p)
            {
                _alloc.destroy(p); // 调用对象的析构函数, 把_frist指针指向的数组 的有效元素进行析构操作
            }

            //  释放_first指针指向的数组内存
            _alloc.deallocate(_first, _last - _first);

            int size = other._end - other._first; // other._end - other._first 空间大小
            _first = new T[size];
            int len = other._last - other._first; // other._last - other._first 有效元素的个数
            for (int i = 0; i < len; i++)
            {
                // _first[i] = other._first[i];
                _alloc.construct(_first + i, other._first[i]);
            }
            _last = _first + len;
            _end = _first + size;
        }
        std::cout << "operator=(const vector<T> &other)" << std::endl;
        return *this;
    }

    // 向容器末尾添加元素。在vector底层的内存上拷贝构造一个新的对象，值跟value一样
    void push_back(const T &value)
    {
        if (full())
        {
            expand();
        }
        // *_last++ = value;
        _alloc.construct(_last, value); // _last指针指向的内存 构造一个值为val的对象
        ++_last;
    }

    // 从容器末尾删除元素
    void pop_back()
    {
        if (empty())
        {
            throw std::out_of_range("empty");
        }
        // --_last;
        // 不仅要_last指针--， 还要析构删除的元素
        std::cout << "   --_last之前的： " << _last << "\n";
        --_last;
        std::cout << "   --_last之后的： " << _last << "\n";
        _alloc.destroy(_last);
    }
    // 判断容器是否已满
    bool full() const
    {
        return _last == _end;
    }

    // 判断容器是否为空
    bool empty() const
    {
        return _first == _last;
    }

    // 返回容器末尾的元素值
    T &back()
    {
        if (empty())
        {
            throw std::out_of_range("empty");
        }
        return *(_last - 1);
    }
    // 返回容器元素个数
    int size() const
    {
        return _last - _first;
    }

private:
    T *_first; // 指向数组起始位置
    T *_last;  // 指向数组中有效元素的后继位置
    T *_end;   // 指向数组空间的后继位置

    Allocator<T> _alloc; // 定义容器的空间配置器对象

    // 容器的二倍扩容操作
    void expand()
    {
        int size = _end - _first;
        // T *ptmp = new T[2 * size];
        T *ptmp = _alloc.allocate(2 * size);

        for (int i = 0; i < size; i++)
        {
            // ptmp[i] = _first[i];
            _alloc.construct(ptmp + i, _first[i]);
        }
        // delete[] _first;
        for (T *p = _first; p != _last; p++)
        {
            _alloc.destroy(p);
        }
        _alloc.deallocate(_first, _last - _first);

        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;

        std::cout << "expand" << std::endl;
    }
};

int main()
{
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::cout << "\n==========\n";
    v.pop_back();
    std::cout << "\n==========\n";

    return 0;
}
