namespace mgd
{
    typedef uint32_t code_point_t;
    
    int utf8_code_point_size(code_point_t u)
    {
        if (u < 0x80)
            return 1;
        if (u < 0x800)
            return 2;
        if (u < 0x10000)
            return 3;
        if (u < 0x110000)
            return 4;
        return -1; // invalid
    }
    
    int utf8_sequence_size(uint8_t byte)
    {
        if (byte < 0x80)
            return 1;
        if (byte < 0xC0)
            return 0; // continuation byte
        if (byte < 0xE0)
            return 2;
        if (byte < 0xF0)
            return 3;
        if (byte < 0xF8)
            return 4;
        return -1; // unknown byte
    }
    
    static const code_point_t replacement_character = 0xFFFD;
    static const int utf8_continuation_payload_size = 6;
    static const uint8_t utf8_continuation_mask = 0x3F;
    
    template <class Iterator>
    class utf8_decoder: public std::iterator<std::forward_iterator_tag, code_point_t, ptrdiff_t, void, void>
    {
        Iterator _it, _end;
    public:
        typedef Iterator iterator_type;
        
        utf8_decoder(): _it(), _end() {}
        utf8_decoder(Iterator begin, Iterator end): _it(begin), _end(end) {}
        bool operator==(const utf8_decoder & it) const { return _it == it._it; }
        bool operator!=(const utf8_decoder & it) const { return _it != it._it; }
        utf8_decoder & operator++() { if ((_it = end()) == _end) _it = _end = Iterator(); return *this; }
        utf8_decoder operator++(int) { utf8_decoder it(*this); operator++(); return it; }
        Iterator begin() const { return _it; }
        
        Iterator end() const
        {
            //TRECE;
            if (_it == _end)
                return _end;
            Iterator it = _it;
            for (int len = utf8_sequence_size(*it++); len > 1 && it != _end && !utf8_sequence_size(*it); --len, ++it);
            return it;
        }
        
        code_point_t operator*() const
        {
            //TRACE;
            code_point_t u = static_cast<uint8_t>(*_it);
            int len = utf8_sequence_size(u);
            if (len <= 0) // continuation or invalid byte
                return replacement_character;
            if (len == 1) // this byte only
                return u;
            Iterator e = end();
            if (std::distance(_it, e) != len)
                return replacement_character;
            u &= ((1 << (7 - len)) - 1); // remove sequence size info
            Iterator it = _it;
            for (++it; it != e; ++it)
            {
                u <<= utf8_continuation_payload_size;
                u |= static_cast<uint8_t>(*it) & utf8_continuation_mask;
            }
            // invalid code point or illegally encoded as sequence
            return utf8_code_point_size(u) == len ? u : replacement_character;
        }
    };
    
    template <class Container, class Iterator>
    class utf8_encoder: public std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        Container & _container;
        Iterator _position;
    public:
        utf8_encoder(Container & container, const Iterator & position): 
            _container(container), _position(position) {}
        utf8_encoder & operator*() { return *this; }
        utf8_encoder & operator++() { return *this; }
        utf8_encoder operator++(int) { return *this; }
        
        utf8_encoder & operator=(code_point_t u)
        {
            int len = utf8_code_point_size(u);
            uint8_t sequence[4];
            if (len == -1)
            {
                u = replacement_character;
                len = utf8_code_point_size(u);
            }
            if (len == 1)
            {
                sequence[0] = u;
            }
            else
            {
                for (int i = len - 1; i; --i)
                {
                    sequence[i] = (u & utf8_continuation_mask) | 0x80;
                    u >>= utf8_continuation_payload_size;
                }
                sequence[0] = u | ~((1 << (8 - len)) - 1); // add sequence size info
            }
            for (int i = 0; i < len; ++i)
                _position = ++_container.insert(_position, sequence[i]);
            return *this;
        }
    };
}

namespace std
{
    template <class Iterator>
    typename std::iterator_traits<mgd::utf8_decoder<Iterator> >::difference_type distance(mgd::utf8_decoder<Iterator> first, mgd::utf8_decoder<Iterator> last)
    {
        return std::count_if(first.begin(), last.begin(), mgd::utf8_sequence_size);
    }
}
