//! \file
#ifndef YACK_NWSORST_senso17_INCLUDED
#define YACK_NWSORST_senso17_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso17
    class senso17 : public pairs<17,0x0047> {
      public:
        virtual ~senso17() throw(); //!< cleanup
        explicit senso17() throw(); //!< setup  
        static const char   sid[];        //!< senso17
        static const size_t lhs[0x0047];  //!< lhs
        static const size_t rhs[0x0047];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso17);
    };
  }
}
#endif
