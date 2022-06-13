//! \file
#ifndef YACK_NWSORST_senso09_INCLUDED
#define YACK_NWSORST_senso09_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso09
    class senso09 : public pairs<9,0x0019> {
      public:
        virtual ~senso09() throw(); //!< cleanup
        explicit senso09() throw(); //!< setup  
        static const char   sid[];        //!< senso09
        static const size_t lhs[0x0019];  //!< lhs
        static const size_t rhs[0x0019];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso09);
    };
  }
}
#endif
