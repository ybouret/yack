//! \file
#ifndef YACK_NWSORST_senso14_INCLUDED
#define YACK_NWSORST_senso14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso14
    class senso14 : public swaps {
      public:
        virtual ~senso14() noexcept; //!< cleanup
        explicit senso14() noexcept; //!< setup  
        static const char   sid[];        //!< senso14
        static const size_t lhs[0x0033];  //!< lhs
        static const size_t rhs[0x0033];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso14);
    };
  }
}
#endif
