//! \file
#ifndef YACK_NWSORST_senso16_INCLUDED
#define YACK_NWSORST_senso16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso16
    class senso16 : public swaps {
      public:
        virtual ~senso16() throw(); //!< cleanup
        explicit senso16() throw(); //!< setup  
        static const char   sid[];        //!< senso16
        static const size_t lhs[0x003c];  //!< lhs
        static const size_t rhs[0x003c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso16);
    };
  }
}
#endif
