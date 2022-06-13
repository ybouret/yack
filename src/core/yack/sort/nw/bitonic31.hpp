//! \file
#ifndef YACK_NWSORST_bitonic31_INCLUDED
#define YACK_NWSORST_bitonic31_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic31
    class bitonic31 : public pairs<31,0x00e1> {
      public:
        virtual ~bitonic31() throw(); //!< cleanup
        explicit bitonic31() throw(); //!< setup  
        static const char   sid[];        //!< bitonic31
        static const size_t lhs[0x00e1];  //!< lhs
        static const size_t rhs[0x00e1];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic31);
    };
  }
}
#endif
