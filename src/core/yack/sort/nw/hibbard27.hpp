//! \file
#ifndef YACK_NWSORST_hibbard27_INCLUDED
#define YACK_NWSORST_hibbard27_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard27
    class hibbard27 : public swaps {
      public:
        virtual ~hibbard27() throw(); //!< cleanup
        explicit hibbard27() throw(); //!< setup  
        static const char   sid[];        //!< hibbard27
        static const size_t lhs[0x00ad];  //!< lhs
        static const size_t rhs[0x00ad];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard27);
    };
  }
}
#endif
