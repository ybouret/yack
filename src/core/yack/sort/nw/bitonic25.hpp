//! \file
#ifndef YACK_NWSORST_bitonic25_INCLUDED
#define YACK_NWSORST_bitonic25_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic25
    class bitonic25 : public pairs<25,0x0099> {
      public:
        virtual ~bitonic25() throw(); //!< cleanup
        explicit bitonic25() throw(); //!< setup  
        static const char   sid[];        //!< bitonic25
        static const size_t lhs[0x0099];  //!< lhs
        static const size_t rhs[0x0099];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic25);
    };
  }
}
#endif
