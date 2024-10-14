#include <iostream>

/**
 * 类模板-> 实现一个c++ STL 里面的顺序容器vector 向量容器
 */

template <typename T>
class vector
{
public:
    vector<T>(int size = 10)
    {
        _first = new T[size];
        _last = _first;
        _end = _first + size;
    }

    ~vector<T>()
    {
        // 析构容器有效的元素，然后释放容器的堆内存
        delete[] _first;
        _first = nullptr;
        _last = nullptr;
        _end = nullptr;
        std::cout << "~vector()" << std::endl;
    }

    // 自定义拷贝构造函数
    vector<T>(const vector<T> &other)
    {
        int size = other._end - other._first; // other._end - other._first 空间大小
        _first = new T[size];
        int len = other._last - other._first; // other._last - other._first 有效元素的个数
        for (int i = 0; i < len; i++)
        {
            _first[i] = other._first[i];
        }

        _last = _first + len;
        _end = _first + size;
        std::cout << "vector(const vector<T> &other)" << std::endl;
    }

    // 赋值重载
    vector<T> &operator=(const vector<T> &other)
    {
        if (this != &other)
        {
            delete[] _first;
            int size = other._end - other._first; // other._end - other._first 空间大小
            _first = new T[size];
            int len = other._last - other._first; // other._last - other._first 有效元素的个数
            for (int i = 0; i < len; i++)
            {
                _first[i] = other._first[i];
            }
            _last = _first + len;
            _end = _first + size;
        }
        std::cout << "operator=(const vector<T> &other)" << std::endl;
        return *this;
    }

    // 向容器末尾添加元素
    void push_back(const T &value)
    {
        if (full())
        {
            expand();
        }
        *_last++ = value;
    }

    // 从容器末尾删除元素
    void pop_back()
    {
        if (empty())
        {
            throw std::out_of_range("empty");
        }
        // 指针倒退一下
        --_last;
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

    // 容器的二倍扩容操作
    void expand()
    {
        int size = _end - _first;
        T *ptmp = new T[2 * size];
        for (int i = 0; i < size; i++)
        {
            ptmp[i] = _first[i];
        }
        delete[] _first;
        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;

        std::cout << "expand" << std::endl;
    }
};

int main()
{
    vector<int> v;
    for (int i = 0; i < 20; i++)
    {
        v.push_back(i);
    }

    while (!v.empty())
    {
        std::cout << v.back() << std::endl;
        v.pop_back();

    }
    
    return 0;
}
