#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <algorithm>
#include <stddef.h>

template <typename T, typename Alloc = std::allocator<T> >
class Vector
{
    friend bool operator==(const Vector &lhs, const Vector &rhs)
    {
        return lhs.size() == rhs.size() && 
            std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    class reverse_iterator;
    class const_reverse_iterator;
public:
    typedef T value_type;
    typedef T *iterator;
    typedef const T * const_iterator;
    typedef reverse_iterator reverse_iterator;
    typedef const_reverse_iterator const_reverse_iterator;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Alloc allocator_type;

private:
    class reverse_iterator
    {
    public:
        reverse_iterator(iterator it = NULL) :current_(it) { }
        iterator base() const { return current_; }
        reverse_iterator &operator++()
        {
            --current_;
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator temp(*this);
            --current_;
            return temp;
        }
        reverse_iterator &operator--()
        {
            ++current_;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator temp(*this);
            ++current_;
            return temp;
        }
        reference operator*()
        {
            return *(current_ - 1);
        }
        const_reference operator*() const
        {
            return *(current_ - 1);
        }

        pointer operator->()
        {
            return current_ - 1;
        }
        const_pointer operator->() const
        {
            return current_ - 1;
        }

        friend bool operator==(reverse_iterator i,  reverse_iterator j)
        {
            return i.current_ == j.current_;
        }
        friend bool operator!=(reverse_iterator i,  reverse_iterator j)
        {
            return i.current_ != j.current_;
        }

        friend difference_type operator-(reverse_iterator i,  reverse_iterator j)
        {
            return i.current_ - j.current_;
        }

    private:
        iterator current_; //物理位置
    };

    class const_reverse_iterator
    {
    public:
        const_reverse_iterator(const_iterator it = NULL) :current_(it) { }
        //提供从reverse_iterator 到 const_reverse_iterator的转换
        const_reverse_iterator(reverse_iterator it) :current_(it.base()) { }
        const_iterator base() const { return current_; }
        const_reverse_iterator &operator++()
        {
            --current_;
            return *this;
        }
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator temp(*this);
            --current_;
            return temp;
        }
        const_reverse_iterator &operator--()
        {
            ++current_;
            return *this;
        }
        const_reverse_iterator operator--(int)
        {
            const_reverse_iterator temp(*this);
            ++current_;
            return temp;
        }
        const_reference operator*() const
        {
            return *(current_ - 1);
        }
        const_pointer operator->() const
        {
            return current_ - 1;
        }

        friend bool operator==(const_reverse_iterator i,  const_reverse_iterator j)
        {
            return i.current_ == j.current_;
        }
        friend bool operator!=(const_reverse_iterator i,  const_reverse_iterator j)
        {
            return i.current_ != j.current_;
        }

        friend difference_type operator-(const_reverse_iterator i,  const_reverse_iterator j)
        {
            return i.current_ - j.current_;
        }

    private:
        const_iterator current_; //物理位置
    };

public:

    Vector() { create(); }
    Vector(size_type n, const T &val = T()) 
    { create(n, val); }

    template <typename In>
    Vector(In i, In j) //一段迭代器去初始化容器
    {
        create(i, j);
    }

    Vector(const Vector &v)
    { create(v.begin(), v.end()); }
    Vector &operator=(const Vector &v);
    ~Vector() { uncreate(); }

    template <typename In>
    void assign(In i, In j)
    {
        uncreate();
        create(i, j);
    }
    void assign(size_type n, const T &val)
    {
        uncreate();
        create(n, val);
    }

    reference operator[] (size_type n) { return data_[n]; }
    const_reference operator[] (size_type n) const { return data_[n]; }

    reference at(size_type n) { return data_[n]; }
    const_reference at(size_type n) const { return data_[n]; }

    reference front() { return *begin(); }
    reference back() { return *rbegin(); }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *rbegin(); }

    void push_back(const T &t);

    void swap(Vector &other)
    {
        std::swap(data_, other.data_);
        std::swap(avail_, other.avail_);
        std::swap(limit_, other.limit_);
    }


    size_type size() const { return avail_ - data_; }
    size_type capacity() const { return limit_ - data_; }

    iterator begin() { return data_; }
    iterator end() { return avail_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return avail_; }

    reverse_iterator rbegin() { return reverse_iterator(avail_); }
    reverse_iterator rend() { return reverse_iterator(data_); }
    const_reverse_iterator rbegin() const 
    { return const_reverse_iterator(avail_); }
    const_reverse_iterator rend() const 
    { return const_reverse_iterator(data_); }

    allocator_type get_allocator() const
    { return alloc_; }

private:
    iterator data_; //数组的首元素
    iterator avail_; //最后一个元素的下一个位置
    iterator limit_; //最后一块内存的下一个位置

    std::allocator<T> alloc_; //内存分配器

    //为底层的数组开辟内存空间，并执行相应的初始化
    void create();
    void create(size_type, const T &);
    template <typename In>
    void create(In, In);

    //删除数组中的元素，并且释放内存
    void uncreate();

    //用于push_back函数
    void grow();
    void unCheckedAppend(const T &);
};

template <typename T, typename Alloc>
Vector<T, Alloc> &Vector<T, Alloc>::operator=(const Vector &rhs)
{
    if(this != &rhs)
    {
        uncreate();
        create(rhs.begin(), rhs.end());
    }
    return *this;
}


template <typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const T &t)
{
    if(avail_ == limit_) // full
        grow();
    unCheckedAppend(t);
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::create()
{
    data_ = avail_ = limit_ = NULL;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::create(size_type n, const T &val)
{
    //分配内存
    data_ = alloc_.allocate(n);
    //执行构造函数 拷贝构造函数
    std::uninitialized_fill(data_, data_ + n, val);
    avail_ = limit_ = data_ + n;

    //为什么不适用new？
}


template <typename T, typename Alloc>
template <typename In>
void Vector<T, Alloc>::create(In i, In j)
{
    //分配内存
    data_ = alloc_.allocate(j-i);
    //执行构造函数 copy
    avail_ = limit_ = std::uninitialized_copy(i, j, data_);
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::uncreate()
{
    //先执行析构函数

    if(data_)
    {
        iterator it(avail_); //初始
        while(it != data_)
            alloc_.destroy(--it);
    }

    //释放内存
    alloc_.deallocate(data_, limit_ - data_);

    data_ = limit_ = avail_ = NULL;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::grow()
{
    //确定size
    size_type current_size = limit_ - data_; //当前大小
    size_type new_size = (current_size*2 > 1) ?  current_size*2 : 1;

    //申请内存并构造
    iterator new_data = alloc_.allocate(new_size);
    iterator new_avail =  uninitialized_copy(data_, avail_, new_data);

    //释放原来的元素
    uncreate();

    //重置指针
    data_ = new_data;
    avail_ = new_avail;
    limit_ = data_ + new_size;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::unCheckedAppend(const T &val)
{
    alloc_.construct(avail_++, val); //插入新的元素
}




#endif  /* VECTOR_HPP */

