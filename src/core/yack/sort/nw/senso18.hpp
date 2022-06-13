//! \file
#ifndef YACK_NWSORST_senso18_INCLUDED
#define YACK_NWSORST_senso18_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! senso18
    class senso18 : public pairs<18,0x004e> {
      public:
        virtual ~senso18() throw(); //!< cleanup
        explicit senso18() throw(); //!< setup  
        static const char   sid[];        //!< senso18
        static const size_t lhs[0x004e];  //!< lhs
        static const size_t rhs[0x004e];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso18);
    };
  }
}
#endif
