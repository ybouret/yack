//! \file
#ifndef YACK_NWSORST_senso13_INCLUDED
#define YACK_NWSORST_senso13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso13
    class senso13 : public swaps {
      public:
        virtual ~senso13() noexcept; //!< cleanup
        explicit senso13() noexcept; //!< setup  
        static const char   sid[];        //!< senso13
        static const size_t lhs[0x002d];  //!< lhs
        static const size_t rhs[0x002d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso13);
    };
  }
}
#endif
