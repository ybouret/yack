//! \file
#ifndef YACK_NWSORST_senso14_INCLUDED
#define YACK_NWSORST_senso14_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso14
    class senso14 : public pairs<14,0x0033> {
      public:
        virtual ~senso14() throw(); //!< cleanup
        explicit senso14() throw(); //!< setup  
        static const char   sid[];        //!< senso14
        static const size_t lhs[0x0033];  //!< lhs
        static const size_t rhs[0x0033];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso14);
    };
  }
}
#endif
