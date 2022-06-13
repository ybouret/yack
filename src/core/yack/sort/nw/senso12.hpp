//! \file
#ifndef YACK_NWSORST_senso12_INCLUDED
#define YACK_NWSORST_senso12_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso12
    class senso12 : public pairs<12,0x0027> {
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
