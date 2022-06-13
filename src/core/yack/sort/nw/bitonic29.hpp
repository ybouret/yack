//! \file
#ifndef YACK_NWSORST_bitonic29_INCLUDED
#define YACK_NWSORST_bitonic29_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic29
    class bitonic29 : public pairs<29,0x00c6> {
      public:
        virtual ~bitonic29() throw(); //!< cleanup
        explicit bitonic29() throw(); //!< setup  
        static const char   sid[];        //!< bitonic29
        static const size_t lhs[0x00c6];  //!< lhs
        static const size_t rhs[0x00c6];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic29);
    };
  }
}
#endif
