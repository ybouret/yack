//! \file
#ifndef YACK_NWSORST_senso12_INCLUDED
#define YACK_NWSORST_senso12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso12
    class senso12 : public swaps {
      public:
        virtual ~senso12() throw(); //!< cleanup
        explicit senso12() throw(); //!< setup  
        static const char   sid[];        //!< senso12
        static const size_t lhs[0x0027];  //!< lhs
        static const size_t rhs[0x0027];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso12);
    };
  }
}
#endif
