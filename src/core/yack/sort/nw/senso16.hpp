//! \file
#ifndef YACK_NWSORST_senso16_INCLUDED
#define YACK_NWSORST_senso16_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso16
    class senso16 : public pairs<16,0x003c> {
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
