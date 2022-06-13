//! \file
#ifndef YACK_NWSORST_senso22_INCLUDED
#define YACK_NWSORST_senso22_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso22
    class senso22 : public pairs<22,0x006c> {
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
