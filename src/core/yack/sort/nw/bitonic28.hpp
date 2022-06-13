//! \file
#ifndef YACK_NWSORST_bitonic28_INCLUDED
#define YACK_NWSORST_bitonic28_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic28
    class bitonic28 : public pairs<28,0x00ba> {
      public:
        virtual ~bitonic28() throw(); //!< cleanup
        explicit bitonic28() throw(); //!< setup  
        static const char   sid[];        //!< bitonic28
        static const size_t lhs[0x00ba];  //!< lhs
        static const size_t rhs[0x00ba];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic28);
    };
  }
}
#endif
