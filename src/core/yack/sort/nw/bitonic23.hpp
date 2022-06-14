//! \file
#ifndef YACK_NWSORST_bitonic23_INCLUDED
#define YACK_NWSORST_bitonic23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic23
    class bitonic23 : public swaps {
      public:
        virtual ~bitonic23() throw(); //!< cleanup
        explicit bitonic23() throw(); //!< setup  
        static const char   sid[];        //!< bitonic23
        static const size_t lhs[0x0085];  //!< lhs
        static const size_t rhs[0x0085];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic23);
    };
  }
}
#endif
