#pragma once

#include <iterator>

#define FOREACH(value, range) \
    for (const foreach::iterator &_foreach_it = foreach::begin(range); !foreach::done(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range)); foreach::next(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range))) \
        for (value = foreach::get(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range)); !_foreach_it.done; _foreach_it.done = true)

#define FOREACH_REVERSE(value, range) \
    for (const foreach::iterator &_foreach_it = foreach::rbegin(range); !foreach::done(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range)); foreach::next(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range), true)) \
        for (value = foreach::get(_foreach_it, FOREACH_GET_ITERATOR_TYPE(range)); !_foreach_it.done; _foreach_it.done = true)

#define FOREACH_GET_ITERATOR_TYPE(expr) (true ? foreach::get_iterator_type() : foreach::get_iterator_type::range(foreach::begin(expr)))

namespace foreach
{
    // iterator
    
    struct iterator
    {
        iterator(): done(false) {}
        mutable bool done;
    };

    template <class Iterator> struct range_iterator : public iterator
    {
        range_iterator(Iterator begin, Iterator end): begin(begin), end(end) {}
        mutable Iterator begin, end;
    };
    
    // type
        
    template <class T> struct iterator_type {};
    
    struct get_iterator_type
    {
        template <class Iterator> static iterator_type<Iterator> range(const range_iterator<Iterator> &);        
        template <class T> operator iterator_type<T>() const { return iterator_type<T>(); }
    };
    
    // begin
    
    template <class Container> range_iterator<typename Container::const_iterator> begin(const Container &c)
    {
        return range_iterator<typename Container::const_iterator>(c.begin(), c.end());
    }
    
    template <class Container> range_iterator<typename Container::const_reverse_iterator> rbegin(const Container &c)
    {
        return range_iterator<typename Container::const_reverse_iterator>(c.rbegin(), c.rend());
    }

    template <class Container> range_iterator<typename Container::iterator> begin(Container &c)
    {
        return range_iterator<typename Container::iterator>(c.begin(), c.end());
    }
    
    template <class Container> range_iterator<typename Container::reverse_iterator> rbegin(Container &c)
    {
        return range_iterator<typename Container::reverse_iterator>(c.rbegin(), c.rend());
    }

    template <class Iterator> range_iterator<Iterator> begin(const std::pair<Iterator, Iterator> &r)
    {
        return range_iterator<Iterator>(r.first, r.second);
    }
    
    template <class Iterator> range_iterator<Iterator> rbegin(std::pair<Iterator, Iterator> r)
    {
        return range_iterator<Iterator>(--r.second, --r.first);
    }
    
    template <class T> range_iterator<const T *> begin(const T t[])
    {
        return range_iterator<const T *>(t, t + sizeof(t) / sizeof(T) + 1);
    }
    
    template <class T> range_iterator<const T *> rbegin(const T t[])
    {
        return range_iterator<const T *>(t + sizeof(t) / sizeof(T), t - 1);
    }
    
    template <class T> range_iterator<T*> begin(T t[])
    {
        return range_iterator<T*>(t, t + sizeof(t) / sizeof(T) + 1);
    }
    
    template <class T> range_iterator<T*> rbegin(T t[])
    {
        return range_iterator<T*>(t + sizeof(t) / sizeof(T), t - 1);
    }
    
    range_iterator<const char *> begin(const char *str)
    {
        return range_iterator<const char *>(str, str + std::char_traits<char>::length(str));
    }
    
    range_iterator<const char *> rbegin(const char *str)
    {
        return range_iterator<const char *>(str + std::char_traits<char>::length(str) - 1, str - 1);
    }
    
    range_iterator<char *> begin(char *str)
    {
        return range_iterator<char *>(str, str + std::char_traits<char>::length(str));
    }

    range_iterator<char *> rbegin(char *str)
    {
        return range_iterator<char *>(str + std::char_traits<char>::length(str) - 1, str - 1);
    }
    
    range_iterator<const wchar_t *> begin(const wchar_t *str)
    {
        return range_iterator<const wchar_t *>(str, str + std::char_traits<wchar_t>::length(str));
    }

    range_iterator<const wchar_t *> rbegin(const wchar_t *str)
    {
        return range_iterator<const wchar_t *>(str + std::char_traits<wchar_t>::length(str) - 1, str - 1);
    }
    
    range_iterator<wchar_t *> begin(wchar_t *str)
    {
        return range_iterator<wchar_t *>(str, str + std::char_traits<wchar_t>::length(str));
    }

    range_iterator<wchar_t *> rbegin(wchar_t *str)
    {
        return range_iterator<wchar_t *>(str + std::char_traits<wchar_t>::length(str) - 1, str - 1);
    }
    
    // done
    
    template <class Iterator> bool done(const iterator &it, iterator_type<Iterator>)
    {
        const range_iterator<Iterator> &rit = static_cast<const range_iterator<Iterator> &>(it);
        return rit.done || rit.begin == rit.end;
    }
    
    // next
    
    template <class Iterator> void next(const iterator &it, iterator_type<Iterator>, bool reverse = false)
    {
        if (reverse)
            --static_cast<const range_iterator<Iterator> &>(it).begin;
        else
            ++static_cast<const range_iterator<Iterator> &>(it).begin;
        
        it.done = !it.done;
    }
    
    // get
    
    template <class Iterator> typename std::iterator_traits<Iterator>::reference get(const iterator &it, iterator_type<Iterator>)
    {
        return *static_cast<const range_iterator<Iterator> &>(it).begin;
    }
}
