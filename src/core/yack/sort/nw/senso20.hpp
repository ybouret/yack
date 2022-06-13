//! \file
#ifndef YACK_NWSORST_senso20_INCLUDED
#define YACK_NWSORST_senso20_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso20
    class senso20 : public pairs<20,0x005c> {
      public:
        virtual ~senso20() throw(); //!< cleanup
        explicit senso20() throw(); //!< setup  
        static const char   sid[];        //!< senso20
        static const size_t lhs[0x005c];  //!< lhs
        static const size_t rhs[0x005c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso20);
    };
  }
}
#endif
