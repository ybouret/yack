//! \file
#ifndef YACK_NWSORST_bitonic12_INCLUDED
#define YACK_NWSORST_bitonic12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic12
    class bitonic12 : public swaps {
      public:
        virtual ~bitonic12() throw(); //!< cleanup
        explicit bitonic12() throw(); //!< setup  
        static const char   sid[];        //!< bitonic12
        static const size_t lhs[0x002e];  //!< lhs
        static const size_t rhs[0x002e];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic12);
    };
  }
}
#endif
