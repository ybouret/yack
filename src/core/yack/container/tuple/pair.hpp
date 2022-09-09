//! \file

#ifndef YACK_PAIR_INCLUDED
#define YACK_PAIR_INCLUDED 1

#include "yack/container/tuple.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! prolog to create a pair class
    //
    //__________________________________________________________________________
#define YACK_PAIR_DECL(CATEGORY,CLASS,TYPE1,NAME1,TYPE2,NAME2)                 \
/**/    class CLASS {                                                          \
/**/      public:                                                              \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE1,type1);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE2,type2);                           \
/**/        inline explicit CLASS(param_type1 arg1, param_type2 arg2) :        \
/**/        NAME1(arg1), NAME2(arg2) {}                                        \
/**/        inline ~CLASS() throw()  {}                                        \
/**/        inline CLASS(const CLASS &other) :                                 \
/**/            NAME1(other.NAME1), NAME2(other.NAME2) {}                      \
/**/        inline static const char *__##NAME1() throw() { return #NAME1; }   \
/**/        inline static const char *__##NAME2() throw() { return #NAME2; }   \
/**/        inline friend std::ostream &operator<<(std::ostream &os,           \
/**/                                               const CLASS &self) {        \
/**/            os << '(';                                                     \
/**/            os << __##NAME1() << '=' << self.NAME1;                        \
/**/            os << ',' << __##NAME2() << '=' << self.NAME2;                 \
/**/            os << ')';                                                     \
/**/        return os; }                                                       \
/**/        inline friend bool operator==(const CLASS &LL, const CLASS &RR) {  \
/**/            return (LL.NAME1==RR.NAME1) && (LL.NAME2==RR.NAME2);        }  \
/**/        inline friend bool operator!=(const CLASS &LL, const CLASS &RR) {  \
/**/            return (LL.NAME1!=RR.NAME1) || (LL.NAME2!=RR.NAME2);        }  \
/**/        TYPE1 NAME1;                                                       \
/**/        TYPE2 NAME2
    
    //__________________________________________________________________________
    //
    //
    //! epilog to create a pair class
    //
    //__________________________________________________________________________
#define YACK_PAIR_END(CLASS) \
/**/ private: YACK_DISABLE_ASSIGN(CLASS); }

}

#endif
