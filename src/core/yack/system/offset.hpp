//!\file

#ifndef YACK_OFFSET_INCLUDED
#define YACK_OFFSET_INCLUDED 1



#define YACK_OFFSET_OF(CLASS,MEMBER) ptrdiff_t( (char *)&(static_cast<CLASS *>( (void*)0 )->MEMBER) )

#endif



