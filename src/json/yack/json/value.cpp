
#include "yack/json/value.hpp"


namespace yack
{
    namespace JSON
    {

        Array:: Array() throw() : Array_() {}

        Array:: ~Array() throw() {}

        Array:: Array(const Array &arr) : Array_(arr) {}

        Array & Array:: operator=(const Array &arr)
        {
            {
                Array_       &self = *this;
                const Array_ &that = arr;
                self = that;
            }
            return *this;
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
                    impl = jcopy<String>(impl);
                    break;

                case isArray:
                    impl = jcopy<Array>(impl);
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

        Value:: Value(const double d) throw() :
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
        

    }

}


