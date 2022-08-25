
//! \file

#ifndef YACK_QUAD_INCLUDED
#define YACK_QUAD_INCLUDED 1

#include "yack/container/tuple.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! prolog to create a triple class
    //
    //__________________________________________________________________________
#define YACK_QUAD_DECL(CATEGORY,CLASS,                                         \
/**/                   TYPE1,NAME1,TYPE2,NAME2,TYPE3,NAME3,TYPE4,NAME4)        \
/**/    class CLASS {                                                          \
/**/      public:                                                              \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE1,type1);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE2,type2);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE3,type3);                           \
/**/        YACK_TUPLE_ARGS_##CATEGORY(TYPE4,type4);                           \
/**/        inline explicit CLASS(param_type1 arg1,                            \
/**/                              param_type2 arg2,                            \
/**/                              param_type3 arg3,                            \
/**/                              param_type4 arg4) :                          \
/**/        NAME1(arg1), NAME2(arg2), NAME3(arg3), NAME4(arg4) {}              \
/**/        inline ~CLASS() throw()  {}                                        \
/**/        inline CLASS(const CLASS &other) :                                 \
/**/            NAME1(other.NAME1), NAME2(other.NAME2),                        \
/**/            NAME3(other.NAME3), NAME4(other.NAME4) {}                      \
/**/        inline static const char *__##NAME1() throw() { return #NAME1; }   \
/**/        inline static const char *__##NAME2() throw() { return #NAME2; }   \
/**/        inline static const char *__##NAME3() throw() { return #NAME3; }   \
/**/        inline static const char *__##NAME4() throw() { return #NAME4; }   \
/**/        inline friend std::ostream &operator<<(std::ostream &os,           \
/**/                                               const CLASS &self) {        \
/**/            os << '(';                                                     \
/**/            os << __##NAME1() << '=' << self.NAME1;                        \
/**/            os << ',' << __##NAME2() << '=' << self.NAME2;                 \
/**/            os << ',' << __##NAME3() << '=' << self.NAME3;                 \
/**/            os << ',' << __##NAME4() << '=' << self.NAME4;                 \
/**/            os << ')';                                                     \
/**/        return os; }                                                       \
/**/        TYPE1 NAME1;                                                       \
/**/        TYPE2 NAME2;                                                       \
/**/        TYPE3 NAME3;                                                       \
/**/        TYPE4 NAME4

    //__________________________________________________________________________
    //
    //
    //! epilog to create a triple class
    //
    //__________________________________________________________________________
#define YACK_QUAD_END(CLASS)              \
/**/ private: YACK_DISABLE_ASSIGN(CLASS); }

}

#endif

