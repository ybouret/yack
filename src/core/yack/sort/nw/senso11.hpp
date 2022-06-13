//! \file
#ifndef YACK_NWSORST_senso11_INCLUDED
#define YACK_NWSORST_senso11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso11
    class senso11 : public swaps {
      public:
        virtual ~senso11() throw(); //!< cleanup
        explicit senso11() throw(); //!< setup  
        static const char   sid[];        //!< senso11
        static const size_t lhs[0x0023];  //!< lhs
        static const size_t rhs[0x0023];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso11);
    };
  }
}
#endif
