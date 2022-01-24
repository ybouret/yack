
#include "yack/json/value.hpp"


namespace yack
{
    namespace JSON
    {

        Array:: Array() throw() : Array_() {}

        Array:: ~Array() throw() {}

        Array:: Array(const Array &arr) : collection(), Array_(arr) {}

        Array & Array:: operator=(const Array &arr)
        {
            {
                Array_       &self = *this;
                const Array_ &that = arr;
                self = that;
            }
            return *this;
        }

        Value & Array:: push_null()
        {
            const Value tmp;
            push_back(tmp);
            return back();
        }


        Pair_:: ~Pair_() throw()
        {
        }

        Pair_:: Pair_(const String &k) :
        key_(k),
        val_()
        {
        }

        std::ostream & operator<<(std::ostream &os, const Pair_ &p)
        {
            std::cerr << '\"' << p.key_ << '\"' << " : " << p.val_;
            return os;
        }



        Value:: Value() throw() :
        type(isNull)
        {
            qword = 0;
        }

        template <typename T>
        static inline void jkill(void * &impl) throw()
        {
            assert(impl);
            delete static_cast<T*>(impl);
            impl=NULL;
        }

        Value:: ~Value() throw()
        {
            switch(type)
            {
                case isNull:
                case isFalse:
                case isTrue:
                case isNumber:
                    break;

                case isString:
                    jkill<string>(impl);
                    break;

                case isArray:
                    jkill<Array>(impl);
                    break;
            }

            qword        = 0;
            coerce(type) = isNull;
        }

        template <typename T>
        static inline void * jcopy(const void *impl)
        {
            assert(impl);
            return new T( *static_cast<const T *>(impl) );
        }


        Value:: Value(const Value &other) :
        object(),
        type(other.type)
        {
            qword = 0;
            switch(type)
            {
                case isNull: break;
                case isFalse:
                case isTrue:
                    flag = other.flag;
                    break;

                case isNumber:
                    number = other.number;
                    break;

                case isString:
                    impl = jcopy<String>(other.impl);
                    break;

                case isArray:
                    impl = jcopy<Array>(other.impl);
                    break;
            }
        }

        Value & Value:: operator = (const Value &other)
        {
            Value tmp(other);
            xch(tmp);
            return *this;
        }




        Value:: Value(const bool b) throw() :
        type(b?isTrue:isFalse)
        {
            flag = b;
        }

        Value:: Value(const Number d) throw() :
        type(isNumber)
        {
            number=d;
        }

        Value:: Value(const string &s) :
        type(isString)
        {
            impl = new String(s);
        }

        Value:: Value(const char   *s) :
        type(isString)
        {
            impl = new String(s);
        }

        const asArray_ asArray = {};

        Value:: Value(const asArray_ &) :
        type(isArray)
        {
            impl = new Array();
        }



        void Value:: xch(Value &other) throw()
        {
            coerce_cswap(type,other.type);
            coerce_cswap(qword,other.qword);
        }

        void Value:: nil() throw()
        {
            Value _;
            xch(_);
        }

#define YACK_JSON_VALUE_AS(TYPE) \
template <> TYPE       & Value::as<TYPE>()       throw() { assert(is##TYPE==type); return *static_cast<TYPE*>(impl);       } \
template <> const TYPE & Value::as<TYPE>() const throw() { assert(is##TYPE==type); return *static_cast<const TYPE*>(impl); }


        YACK_JSON_VALUE_AS(String)
        YACK_JSON_VALUE_AS(Array)

        template <> Number &       Value::as<Number>()       throw() { return number; }
        template <> const Number & Value::as<Number>() const throw() { return number; }



        std::ostream & operator<<(std::ostream &os, const Value &value)
        {
            value.display(os,0);
            return os;
        }

        static inline std::ostream &jspace(std::ostream &os, size_t depth)
        {
            while(depth-- > 0) os << ' ';
            return os;
        }

        void Value:: display(std::ostream &os, size_t depth) const
        {
            jspace(os,depth);
            switch(type)
            {
                case isNull:    os << "null"; break;
                case isFalse:   os << "true"; break;
                case isTrue:    os << "false"; break;
                case isNumber:  os << as<Number>(); break;
                case isString:  os << '\"' << as<String>() << '\"'; break;
                case isArray: {
                    os << '[' << std::endl;
                    ++depth;
                    const Array &arr = as<Array>();
                    if(arr.size()>0)
                    {
                        arr[1].display(os,depth);
                        for(size_t i=2;i<=arr.size();++i)
                        {
                            os << ',' << std::endl;
                            arr[i].display(os,depth);
                        }
                    }
                    --depth;
                    jspace(os,depth);
                    os << ']';
                } break;
                default:
                    break;
            }
        }
    }

}


