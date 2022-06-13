//! \file
#ifndef YACK_NWSORST_senso21_INCLUDED
#define YACK_NWSORST_senso21_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso21
    class senso21 : public pairs<21,0x0066> {
      public:
        virtual ~senso21() throw(); //!< cleanup
        explicit senso21() throw(); //!< setup  
        static const char   sid[];        //!< senso21
        static const size_t lhs[0x0066];  //!< lhs
        static const size_t rhs[0x0066];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso21);
    };
  }
}
#endif
