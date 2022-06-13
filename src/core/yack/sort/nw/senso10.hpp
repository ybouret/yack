//! \file
#ifndef YACK_NWSORST_senso10_INCLUDED
#define YACK_NWSORST_senso10_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso10
    class senso10 : public pairs<10,0x001d> {
      public:
        virtual ~senso10() throw(); //!< cleanup
        explicit senso10() throw(); //!< setup  
        static const char   sid[];        //!< senso10
        static const size_t lhs[0x001d];  //!< lhs
        static const size_t rhs[0x001d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso10);
    };
  }
}
#endif
