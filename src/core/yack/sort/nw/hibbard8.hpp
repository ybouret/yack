//! \file
#ifndef YACK_NWSORST_hibbard8_INCLUDED
#define YACK_NWSORST_hibbard8_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard8
    class hibbard8 : public swaps {
      public:
        virtual ~hibbard8() throw(); //!< cleanup
        explicit hibbard8() throw(); //!< setup  
        static const char   sid[];        //!< hibbard8
        static const size_t lhs[0x0013];  //!< lhs
        static const size_t rhs[0x0013];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard8);
    };
  }
}
#endif
