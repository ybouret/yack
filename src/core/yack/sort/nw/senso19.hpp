//! \file
#ifndef YACK_NWSORST_senso19_INCLUDED
#define YACK_NWSORST_senso19_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso19
    class senso19 : public pairs<19,0x0056> {
      public:
        virtual ~senso19() throw(); //!< cleanup
        explicit senso19() throw(); //!< setup  
        static const char   sid[];        //!< senso19
        static const size_t lhs[0x0056];  //!< lhs
        static const size_t rhs[0x0056];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso19);
    };
  }
}
#endif
