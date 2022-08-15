//! \file

#ifndef YACK_PAIR_INCLUDED
#define YACK_PAIR_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
#define YACK_PAIR_DECL(CLASS,TYPE1,NAME1,TYPE2,NAME2) \
class CLASS { \
public:\

#define YACK_PAIR_END(CLASS) \
private: YACK_DISABLE_ASSIGN(CLASS); }

}

#endif

