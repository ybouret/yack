//! \file
#ifndef YACK_NWSORST_senso15_INCLUDED
#define YACK_NWSORST_senso15_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso15
    class senso15 : public pairs<15,0x0038> {
      public:
        virtual ~senso15() throw(); //!< cleanup
        explicit senso15() throw(); //!< setup  
        static const char   sid[];        //!< senso15
        static const size_t lhs[0x0038];  //!< lhs
        static const size_t rhs[0x0038];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso15);
    };
  }
}
#endif
