namespace mgd {
    template <class T> class is_class {
        typedef char no;
        struct yes { no _[2]; };
        template <class U> static yes test(void (U::*)());
        template <class U> static no test(...);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    };
    
    template <bool, class = void> struct enable_if {};
    template <class T> struct enable_if<true, T> { typedef T type; };
    
    class any {
        struct value {
            virtual ~value() = 0;
            virtual value * clone() const = 0;
        };
        template <class V> struct class_value: public value, public V {
            class_value(const V & v): V(v) {}
            value * clone() const { return new class_value(*this); }
        };
        template <class V> struct basic_value: public value {
            basic_value(const V & v): v(v) {}
            value * clone() const { return new basic_value(v); }
            V v;
        };
        std::auto_ptr<value> _v;
        template <class V> friend const V * any_cast(const any * a, typename enable_if<is_class<V>::value>::type* = 0) {
            return a ? dynamic_cast<const V *>(a->_v.get()) : 0;
        }
        template <class V> friend const V * any_cast(const any * a, typename enable_if<!is_class<V>::value>::type* = 0) {
            return a && dynamic_cast<const basic_value<V> *>(a->_v.get()) ? &dynamic_cast<const basic_value<V> *>(a->_v.get())->v : 0;
        }
    public:
        any() {}
        template <class V> any(const V & v, typename enable_if<is_class<V>::value>::type* = 0): _v(new class_value<V>(v)) {}
        template <class V> any(const V & v, typename enable_if<!is_class<V>::value>::type* = 0): _v(new basic_value<V>(v)) {}
        any(const any & a): _v(a._v.get() ? a._v->clone() : 0) {}
        any & operator=(const any & a) { _v.reset(a._v.get() ? a._v->clone() : 0); }
    };
    any::value::~value() {}
    template <class V> V * any_cast(any * a) { return const_cast<V *>(any_cast<V>(const_cast<const any *>(a))); }
    template <class V> V & any_cast(any & a) { return const_cast<V &>(any_cast<V>(const_cast<const any &>(a))); }
    template <class V> const V & any_cast(const any & a) { if (const V * v = any_cast<V>(&a)) return *v; throw std::bad_cast(); }
}

