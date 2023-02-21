//! \file
#ifndef YACK_NWSORST_senso10_INCLUDED
#define YACK_NWSORST_senso10_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso10
    class senso10 : public swaps {
      public:
        virtual ~senso10() noexcept; //!< cleanup
        explicit senso10() noexcept; //!< setup  
        static const char   sid[];        //!< senso10
        static const size_t lhs[0x001d];  //!< lhs
        static const size_t rhs[0x001d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso10);
    };
  }
}
#endif
