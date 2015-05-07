#include <iostream>
#include <cctype>
#include <vector>
#include "foreach.hpp"

// requires std::iterator_traits<Iterator>
// requires container::iterator, begin() and end() (for non-const references)
// requires container::const_iterator, begin() const and end() const (for const references and rvalues)

using namespace std;

void const_lvalue(const string &str)
{
    cout << __FUNCTION__ << ": ";
    
    FOREACH_REVERSE(const char &c, str)
    {
        if (c == 'l')
            continue;
        
        cout << c;
        
        if (c == 'e')
            break;
    }
    
    cout << endl;
}

void nonconst_lvalue(string &str)
{
    cout << __FUNCTION__ << ": ";
    FOREACH_REVERSE(char &c, str)
        c = toupper(c);
    cout << str << endl;
}

const char *s = "Hello, World!";

void rvalue()
{
    cout << __FUNCTION__ << ": ";
    char c;
    FOREACH_REVERSE(c, string(s))
        cout << c;
    cout << endl;
}

void range(string & str)
{
    cout << __FUNCTION__ << ": ";
    FOREACH_REVERSE(char &c, make_pair(str.begin(), str.end()))
        c = tolower(c);
    cout << str << endl;
}

void cstr()
{
    cout << __FUNCTION__ << ": ";
    FOREACH_REVERSE(char c, "Hello, World!")
        cout << c;
    cout << endl;
    
    string str("Hello, World!");
    FOREACH_REVERSE(char &c, const_cast<char *>(str.c_str()))
        c = toupper(c);
    cout << str << endl;
}

void array()
{
    cout << __FUNCTION__ << ": ";
    const int array1[] = {1, 2, 3};
    FOREACH_REVERSE(int i, array1)
        cout << i << ' ';
    cout << endl;
    
    int array2[] = {1,2,3};
    FOREACH_REVERSE(int &i, array2)
        ++i;
    FOREACH_REVERSE(int i, array2)
        cout << i << ' ';
    cout << endl;    
}

struct Integers
{
    Integers(const Integers& i): ints(i.ints)
    {
        cerr << __PRETTY_FUNCTION__ << endl;
    }
    
    std::vector<int> ints;
    
    Integers(int first, int last)
    {
        for (int i = first; i <= last; ++i)
            ints.push_back(i);
    }
    
    typedef int * iterator;
    typedef const int * const_iterator;
    typedef int * reverse_iterator;
    typedef const int * const_reverse_iterator;
    
    iterator begin() { return &ints.front(); }
    const_iterator begin() const { return &ints.front(); }
    iterator end() { return &ints.back() + 1; }
    const_iterator end() const { return &ints.back() + 1; }
    
    reverse_iterator rbegin() { return &ints.back(); }
    const_reverse_iterator rbegin() const { return &ints.back(); }
    reverse_iterator rend() { return &ints.front() - 1; }
    const_reverse_iterator rend() const { return &ints.front() - 1; }
};

void customClass()
{
    cout << __FUNCTION__ << ": ";
    FOREACH_REVERSE(int i, Integers(20, 30))
        cout << i << ' ';
    cout << endl;
}

int main()
{
    string str(s);
    const_lvalue(str);
    nonconst_lvalue(str);
    rvalue();
    range(str);
    cstr();
    array();
    customClass();
}
