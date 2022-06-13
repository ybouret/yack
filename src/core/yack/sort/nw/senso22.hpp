//! \file
#ifndef YACK_NWSORST_senso22_INCLUDED
#define YACK_NWSORST_senso22_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso22
    class senso22 : public swaps {
      public:
        virtual ~senso22() throw(); //!< cleanup
        explicit senso22() throw(); //!< setup  
        static const char   sid[];        //!< senso22
        static const size_t lhs[0x006c];  //!< lhs
        static const size_t rhs[0x006c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso22);
    };
  }
}
#endif
