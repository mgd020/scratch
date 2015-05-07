#include <iostream>
using namespace std;

namespace variant
{
    namespace detail
    {
        template <int a, int b>
        struct max
        {
            static const int value = a > b ? a : b;
        };
        
        template <class T, class... R>
        struct variant
        {
            static const int max_size = max<sizeof(T), variant<R...>::max_size>::value;
            static const int length = variant<R...>::length + 1;
            
            static int default_construct(char *data)
            {
                return variant<T>::default_construct(data);
            }
            
            static int construct(char *data, const T & t)
            {
                return variant<T>::construct(data, t);
            }
            
            template <class U>
            static int construct(char *data, const U & u)
            {
                return variant<R...>::construct(data, u) + 1;
            }
            
            static int construct(char *data, int index, const char *from)
            {
                if (index)
                    return variant<R...>::construct(data, --index, from) + 1;
                return variant<T>::construct(data, 0, from);
            }
            
            static void destruct(char *data, int index)
            {
                if (index)
                    return variant<R...>::destruct(data, --index);
                return variant<T>::destruct(data, index);
            }
            
            static int index(T * t)
            {
                return variant<T>::index(t);
            }
            
            template <class U>
            static int index(U * u)
            {
                return variant<R...>::index(u) + 1;
            }
        };
        
        template <class T>
        struct variant<T>
        {
            static const int max_size = sizeof(T);
            static const int length = 1;
            
            static int default_construct(char *data)
            {
                new (data) T();
                return 0;
            }
            
            static int construct(char *data, const T & t)
            {
                new (data) T(t);
                return 0;
            }
            
            static int construct(char *data, int, const char *from)
            {
                new (data) T(*reinterpret_cast<const T *>(from));
                return 0;
            }
            
            static void destruct(char *data, int)
            {
                reinterpret_cast<T *>(data)->~T();
            }
            
            static int index(T * t)
            {
                return 0;
            }
        };
    }

    template <class... T>
    class variant
    {
        char data[detail::variant<T...>::max_size];
        int index;
        
    public:
        variant(): index(detail::variant<T...>::default_construct(data))
        {
        }
        
        template <class U>
        variant(const U & u): index(detail::variant<T...>::construct(data, u))
        {
        }
        
        variant(const variant & v): index(detail::variant<T...>::construct(data, v.index, v.data))
        {
        }
        
        template <class U>
        variant & operator=(const U & u)
        {
            detail::variant<T...>::destruct(data, index);
            index = detail::variant<T...>::construct(data, u);
            return *this;
        }
        
        variant & operator=(const variant & v)
        {
            detail::variant<T...>::destruct(data, index);
            index = detail::variant<T...>::construct(data, v.index, v.data);
            return *this;
        }
        
        ~variant()
        {
            detail::variant<T...>::destruct(data, index);
        }
        
        int which() const
        {
            return index;
        }
        
        template <class U>
        friend const U * variant_cast(const variant * v)
        {
            return v && detail::variant<T...>::index((U *)0) == v->index ? reinterpret_cast<const U *>(v->data) : 0;
        }
    };

    template <class U, class... T>
    U * variant_cast(variant<T...> * v)
    {
        return const_cast<U *>(variant_cast<U>(const_cast<const variant<T...> *>(v)));
    }

    template <class U, class... T>
    const U & variant_cast(const variant<T...> & v)
    {
        if (const U * u = variant_cast<U>(&v))
            return u;
        throw std::bad_cast();
    }

    template <class U, class... T>
    U & variant_cast(variant<T...> & v)
    {
        if (U * u = variant_cast<U>(&v))
            return u;
        throw std::bad_cast();
    }
}

using namespace variant;

struct A
{
    A() { cout << __PRETTY_FUNCTION__ << endl; }
    A(const A &) { cout << __PRETTY_FUNCTION__ << endl; }
    A & operator=(const A &) { cout << __PRETTY_FUNCTION__ << endl; return *this; }
    ~A() { cout << __PRETTY_FUNCTION__ << endl; }
};

struct B
{
    B() { cout << __PRETTY_FUNCTION__ << endl; }
    B(const B &) { cout << __PRETTY_FUNCTION__ << endl; }
    B & operator=(const B &) { cout << __PRETTY_FUNCTION__ << endl; return *this; }
    ~B() { cout << __PRETTY_FUNCTION__ << endl; }
};

struct visitor
{
    void operator()(const A &)
    {
        cout << 'A' << endl;
    }
    
    void operator()(const A &)
    {
        cout << 'B' << endl;
    }
};

int main()
{
    {
        variant<A, B> a;
        cout << variant_cast<A>(&a) << endl;
        cout << variant_cast<B>(&a) << endl;
    }
    cout << endl;
    {
        variant<A, B> a = A();
        cout << variant_cast<A>(&a) << endl;
        cout << variant_cast<B>(&a) << endl;
    }
    cout << endl;
    {
        variant<A, B> b = B();
        cout << variant_cast<A>(&b) << endl;
        cout << variant_cast<B>(&b) << endl;
    }
    cout << endl;
    {
        variant<A, B> a;
        cout << variant_cast<A>(&a) << endl;
        cout << variant_cast<B>(&a) << endl;
        cout << __LINE__ << endl;
        variant<A, B> b = a;
        cout << variant_cast<A>(&b) << endl;
        cout << variant_cast<B>(&b) << endl;
    }
    cout << endl;
    {
        variant<A, B> b;
        cout << variant_cast<A>(&b) << endl;
        cout << variant_cast<B>(&b) << endl;
        cout << __LINE__ << endl;
        b = B();
        cout << variant_cast<A>(&b) << endl;
        cout << variant_cast<B>(&b) << endl;
    }
}
