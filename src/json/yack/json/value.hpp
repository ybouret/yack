//! \file


#ifndef YACK_JSON_VALUE_INCLUDED
#define YACK_JSON_VALUE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace JSON
    {

        //______________________________________________________________________
        //
        //
        // global definitions
        //
        //______________________________________________________________________
        class           Value;
        typedef string String; //!< string
        typedef double Number; //!< number

        //______________________________________________________________________
        //
        //
        //! handled types
        //
        //______________________________________________________________________
        enum ValueType
        {
            isNull,       //!< null
            isTrue,       //!< true
            isFalse,      //!< false
            isNumber,     //!< number
            isString,     //!< string
            isArray,      //!< array
            isObject      //!< object
        };

        //______________________________________________________________________
        //
        //
        // Arrays
        //
        //______________________________________________________________________
        typedef int2type<isArray> asArray_; //!< helper for array
        extern const asArray_     asArray;  //!< helper for array
        typedef vector<Value>     Array_;   //!< base class

        //______________________________________________________________________
        //
        //
        //! Array
        //
        //______________________________________________________________________
        class Array : public object, public Array_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Array() throw();         //!< setup
            virtual ~Array() throw();         //!< cleanup
            Array(const Array &);             //!< copy
            Array & operator=(const Array &); //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            Value &push_null(); //!< push a new null value
        };


        //______________________________________________________________________
        //
        //
        // Objects
        //
        //______________________________________________________________________
        class                           Pair_;
        typedef ark_ptr<string,Pair_>   Pair;      //!< shared pair
        typedef suffix_set<string,Pair> Object_;   //!< base class for Object
        typedef int2type<isObject>      asObject_; //!< helper
        extern const asObject_          asObject;  //!< helper

        //______________________________________________________________________
        //
        //
        //! Object
        //
        //______________________________________________________________________
        class Object : public large_object, public Object_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Object() throw();         //!< setup
            virtual ~Object() throw();         //!< cleanup
            Object(const Object &);            //!< copy
            Object & operator=(const Object &);//!< assign
            
        };


        //______________________________________________________________________
        //
        //
        //! Value
        //
        //______________________________________________________________________
        class Value : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Value() throw();                   //!< is_null
            virtual ~Value() throw();          //!< cleanup
            Value(const Value &);              //!< copy
            Value & operator= (const Value &); //!< copy/swap

            Value(const bool)       throw(); //!< true/flase
            Value(const Number)     throw(); //!< number
            Value(const string    &);        //!< string
            Value(const char      *);        //!< string
            Value(const asArray_  &);        //!< new array
            Value(const asObject_ &);        //!< new object

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void nil()        throw(); //!< make nulll
            void xch(Value &) throw(); //!< no-throw exchange

            //! display
            friend std::ostream & operator<<(std::ostream &os, const Value &);

            template <typename T> T       &as() throw();       //!< convert (but boolean)
            template <typename T> const T &as() const throw(); //!< convert (but boolean)

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const ValueType type; //!< the type

        private:
            void *impl;
#if 0
            union
            {
                void    *impl;   //!< for allocated types
                bool     flag;   //!< info
                double   number; //!< number
                uint64_t qword;  //!< to reset
            };
#endif
            
            void display(std::ostream &os, size_t depth) const;
        };


        //______________________________________________________________________
        //
        //
        //! Pair_ for Object
        //
        //______________________________________________________________________
        class Pair_ : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Pair_(const String &); //!< setup with nil
            virtual ~Pair_() throw();       //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string &key() const throw(); //!< for Object
            friend std::ostream & operator<<(std::ostream &, const Pair_ &); //!< helper

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            const String  key_; //!< key for object
            Value         val_; //!< current value

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Pair_);
        };

    }

}

#endif
