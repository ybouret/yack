//! \file
#ifndef YACK_NWSORST_senso17_INCLUDED
#define YACK_NWSORST_senso17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso17
    class senso17 : public swaps {
      public:
        virtual ~senso17() noexcept; //!< cleanup
        explicit senso17() noexcept; //!< setup  
        static const char   sid[];        //!< senso17
        static const size_t lhs[0x0047];  //!< lhs
        static const size_t rhs[0x0047];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso17);
    };
  }
}
#endif
