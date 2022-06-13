//! \file
#ifndef YACK_NWSORST_bitonic7_INCLUDED
#define YACK_NWSORST_bitonic7_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic7
    class bitonic7 : public pairs<7,0x0012> {
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
