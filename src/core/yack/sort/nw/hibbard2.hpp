//! \file
#ifndef YACK_NWSORST_hibbard2_INCLUDED
#define YACK_NWSORST_hibbard2_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard2
    class hibbard2 : public swaps {
      public:
        virtual ~hibbard2() throw(); //!< cleanup
        explicit hibbard2() throw(); //!< setup  
        static const char   sid[];        //!< hibbard2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard2);
    };
  }
}
#endif
