//! \file
#ifndef YACK_NWSORST_bitonic30_INCLUDED
#define YACK_NWSORST_bitonic30_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic30
    class bitonic30 : public pairs<30,0x00d3> {
      public:
        virtual ~bitonic30() throw(); //!< cleanup
        explicit bitonic30() throw(); //!< setup  
        static const char   sid[];        //!< bitonic30
        static const size_t lhs[0x00d3];  //!< lhs
        static const size_t rhs[0x00d3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic30);
    };
  }
}
#endif
