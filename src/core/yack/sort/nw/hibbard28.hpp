//! \file
#ifndef YACK_NWSORST_hibbard28_INCLUDED
#define YACK_NWSORST_hibbard28_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard28
    class hibbard28 : public swaps {
      public:
        virtual ~hibbard28() throw(); //!< cleanup
        explicit hibbard28() throw(); //!< setup  
        static const char   sid[];        //!< hibbard28
        static const size_t lhs[0x00b3];  //!< lhs
        static const size_t rhs[0x00b3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard28);
    };
  }
}
#endif
