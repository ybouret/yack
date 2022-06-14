//! \file
#ifndef YACK_NWSORST_bitonic19_INCLUDED
#define YACK_NWSORST_bitonic19_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic19
    class bitonic19 : public swaps {
      public:
        virtual ~bitonic19() throw(); //!< cleanup
        explicit bitonic19() throw(); //!< setup  
        static const char   sid[];        //!< bitonic19
        static const size_t lhs[0x0062];  //!< lhs
        static const size_t rhs[0x0062];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic19);
    };
  }
}
#endif
