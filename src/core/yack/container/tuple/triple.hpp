//! \file

#ifndef YACK_TRIPLE_INCLUDED
#define YACK_TRIPLE_INCLUDED 1

#include "yack/container/tuple.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! prolog to create a triple class
    //
    //__________________________________________________________________________
#define YACK_TRIPLE_DECL(CATEGORY,CLASS,TYPE1,NAME1,TYPE2,NAME2,TYPE3,NAME3)   \
/**/    class CLASS {                                                          \
/**/      public:                                                              \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE1,type1);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE2,type2);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE3,type3);                           \
/**/        inline explicit CLASS(param_type1 arg1,                            \
/**/                              param_type2 arg2,                            \
/**/                              param_type3 arg3) :                          \
/**/        NAME1(arg1), NAME2(arg2), NAME3(arg3) {}                           \
/**/        inline ~CLASS() throw()  {}                                        \
/**/        inline CLASS(const CLASS &other) :                                 \
/**/            NAME1(other.NAME1), NAME2(other.NAME2), NAME3(other.NAME3) {}  \
/**/        inline static const char *__##NAME1() throw() { return #NAME1; }   \
/**/        inline static const char *__##NAME2() throw() { return #NAME2; }   \
/**/        inline static const char *__##NAME3() throw() { return #NAME3; }   \
/**/        inline friend std::ostream &operator<<(std::ostream &os,           \
/**/                                               const CLASS &self) {        \
/**/            os << '(';                                                     \
/**/            os << __##NAME1() << '=' << self.NAME1;                        \
/**/            os << ',' << __##NAME2() << '=' << self.NAME2;                 \
/**/            os << ',' << __##NAME3() << '=' << self.NAME3;                 \
/**/            os << ')';                                                     \
/**/        return os; }                                                       \
/**/        TYPE1 NAME1;                                                       \
/**/        TYPE2 NAME2;                                                       \
/**/        TYPE3 NAME3

    //__________________________________________________________________________
    //
    //
    //! epilog to create a triple class
    //
    //__________________________________________________________________________
#define YACK_TRIPLE_END(CLASS)              \
/**/ private: YACK_DISABLE_ASSIGN(CLASS); }

}

#endif

