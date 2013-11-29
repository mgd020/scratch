#include <iostream>
#include <stdint.h>
#include <iterator>
#include <iomanip>
#include <vector>
#include <cstdarg>
#include "utf8.hpp"

using namespace std;

template <class T>
bool test(const char *code, const T & a, const T & b = T())
{
    cout << "'" << code << "' got '" << hex << setw(4) << setfill('0') << a;
    if (b == T() || a == b)
    {
        cout << "'" << endl;
        return true;
    }
    cout << "' EXPECTED '" << hex << setw(4) << setfill('0') << b << "'" << endl;
    return false;
}

#define TEST(...) test(#__VA_ARGS__, __VA_ARGS__)

template <class T>
ostream & operator<<(ostream & oss, const std::vector<T> & t)
{
    streamsize w = oss.width();
    oss << setw(1) << "[";
    for (typename std::vector<T>::const_iterator it = t.begin(); it != t.end(); ++it)
    {
        if (it != t.begin())
            oss << ", ";
        oss << setw(w) << *it;
    }
    oss << "]";
    return oss;
}

template <class Decoder>
vector<mgd::code_point_t> decode(const char * str)
{
    vector<mgd::code_point_t> result;
    const char * end = str + strlen(str);
    copy(Decoder(str, end), Decoder(), std::back_inserter(result));
    return result;
}

vector<mgd::code_point_t> make(int len, ...)
{
    va_list ap;
    va_start(ap, len);
    vector<mgd::code_point_t> v;
    while (len--)
        v.push_back(va_arg(ap, mgd::code_point_t));
    va_end(ap);
    return v;
}

vector<uint16_t> make(const char * str)
{
    vector<uint16_t> v;
    for (int i = 0, e = strlen(str); i != e; ++i)
        v.push_back(uint16_t(str[i]) & 0xFF);
    return v;
}

std::vector<uint16_t> encode(const std::vector<mgd::code_point_t> & v)
{
    std::vector<uint16_t> s;
    std::copy(v.begin(), v.end(), mgd::utf8_encoder<std::vector<uint16_t>, std::vector<uint16_t>::iterator>(s, s.end()));
    return s;
}

int main()
{
    TEST(decode<mgd::utf8_decoder<const char *> >("\x41\xE2\x89\xA2\xCE\x91\x2E"), make(4, 0x0041, 0x2262, 0x0391, 0x002E));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4"), make(3, 0xD55C, 0xAD6D, 0xC5B4));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"), make(3, 0x65E5, 0x672C, 0x8A9E));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xEF\xBB\xBF\xF0\xA3\x8E\xB4"), make(2, 0xFEFF, 0x233B4));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xED\xA1\x8C\xED\xBE\xB4"), make(2, 0xD84C, 0xDFB4));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xC0\x80"), make(1, 0xFFFD));
    TEST(decode<mgd::utf8_decoder<const char *> >("\x2F\xC0\xAE\x2E\x2F"), make(4, 0x002F, 0xFFFD, 0x002E, 0x002F));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xAA\xE2\x89\xCE\xFF"), make(4, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD));
    TEST(decode<mgd::utf8_decoder<const char *> >("\xE2\x89"), make(1, 0xFFFD));
    
    TEST(encode(make(4, 0x0041, 0x2262, 0x0391, 0x002E)), make("\x41\xE2\x89\xA2\xCE\x91\x2E"));
    TEST(encode(make(3, 0xD55C, 0xAD6D, 0xC5B4)), make("\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4"));
    TEST(encode(make(3, 0x65E5, 0x672C, 0x8A9E)), make("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"));
    TEST(encode(make(2, 0xFEFF, 0x233B4)), make("\xEF\xBB\xBF\xF0\xA3\x8E\xB4"));
    TEST(encode(make(2, 0xD84C, 0xDFB4)), make("\xED\xA1\x8C\xED\xBE\xB4"));
    TEST(encode(make(1, 0x110000)), make("\xEF\xBF\xBD"));
}
