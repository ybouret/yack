//! \file
#ifndef YACK_NWSORST_hibbard25_INCLUDED
#define YACK_NWSORST_hibbard25_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard25
    class hibbard25 : public pairs<25,0x009a> {
      public:
        virtual ~hibbard25() throw(); //!< cleanup
        explicit hibbard25() throw(); //!< setup  
        static const char   sid[];        //!< hibbard25
        static const size_t lhs[0x009a];  //!< lhs
        static const size_t rhs[0x009a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard25);
    };
  }
}
#endif
