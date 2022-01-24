//! \file


#ifndef YACK_JSON_VALUE_INCLUDED
#define YACK_JSON_VALUE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace JSON
    {

        class           Value;
        typedef string String;
        typedef double Number;

        enum ValueType
        {
            isNull,
            isTrue,
            isFalse,
            isNumber,
            isString,
            isArray
        };

        typedef int2type<isArray> asArray_;
        extern const asArray_     asArray;



        typedef vector<Value> Array_;

        class Array : public Array_
        {
        public:
            explicit Array() throw();
            virtual ~Array() throw();
            Array(const Array &);
            Array & operator=(const Array &);

            Value &push_null();

        };



        class Pair_;
        typedef ark_ptr<string,Pair_> Pair;



        class Value : public object
        {
        public:
            Value() throw(); //!< is_null
            virtual ~Value() throw();
            Value(const Value &);
            Value & operator= (const Value &);

            Value(const bool)       throw();
            Value(const Number)     throw();
            Value(const string   &);
            Value(const char     *);
            Value(const asArray_ &);


            void nil() throw();
            void xch(Value &) throw();

            const ValueType type;

            friend std::ostream & operator<<(std::ostream &os, const Value &);

            template <typename T> T       &as() throw();
            template <typename T> const T &as() const throw();


        private:

            union
            {
                void    *impl;
                bool     flag;
                double   number;
                uint64_t qword;
            };

            void display(std::ostream &os, size_t depth) const;
        };


        class Pair_ : public object, public counted
        {
        public:
            explicit Pair_(const String &);
            virtual ~Pair_() throw();

            const string &key() const throw();

            const String  key_;
            Value         val_;
            friend std::ostream & operator<<(std::ostream &, const Pair_ &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Pair_);
        };

    }

}

#endif
