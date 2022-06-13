//! \file
#ifndef YACK_NWSORST_senso23_INCLUDED
#define YACK_NWSORST_senso23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso23
    class senso23 : public swaps {
      public:
        virtual ~senso23() throw(); //!< cleanup
        explicit senso23() throw(); //!< setup  
        static const char   sid[];        //!< senso23
        static const size_t lhs[0x0076];  //!< lhs
        static const size_t rhs[0x0076];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso23);
    };
  }
}
#endif
