//! \file
#ifndef YACK_NWSORST_bitonic27_INCLUDED
#define YACK_NWSORST_bitonic27_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic27
    class bitonic27 : public pairs<27,0x00ae> {
      public:
        virtual ~bitonic27() throw(); //!< cleanup
        explicit bitonic27() throw(); //!< setup  
        static const char   sid[];        //!< bitonic27
        static const size_t lhs[0x00ae];  //!< lhs
        static const size_t rhs[0x00ae];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic27);
    };
  }
}
#endif
