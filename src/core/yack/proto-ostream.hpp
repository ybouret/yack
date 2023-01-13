//! \file

#ifndef YACK_PROTO_OSTREAM_INCLUDED
#define YACK_PROTO_OSTREAM_INCLUDED 1

#include <iostream>

//! helper to declare specific display
#define YACK_PROTO_OSTREAM(CLASS) \
friend std::ostream & operator<<(std::ostream &, const CLASS &)

#endif

