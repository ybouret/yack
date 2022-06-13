//! \file
#ifndef YACK_NWSORST_bitonic7_INCLUDED
#define YACK_NWSORST_bitonic7_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic7
    class bitonic7 : public swaps {
      public:
        virtual ~bitonic7() throw(); //!< cleanup
        explicit bitonic7() throw(); //!< setup  
        static const char   sid[];        //!< bitonic7
        static const size_t lhs[0x0012];  //!< lhs
        static const size_t rhs[0x0012];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic7);
    };
  }
}
#endif
