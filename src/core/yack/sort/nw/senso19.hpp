//! \file
#ifndef YACK_NWSORST_senso19_INCLUDED
#define YACK_NWSORST_senso19_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso19
    class senso19 : public swaps {
      public:
        virtual ~senso19() throw(); //!< cleanup
        explicit senso19() throw(); //!< setup  
        static const char   sid[];        //!< senso19
        static const size_t lhs[0x0056];  //!< lhs
        static const size_t rhs[0x0056];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso19);
    };
  }
}
#endif
