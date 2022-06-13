//! \file
#ifndef YACK_NWSORST_hibbard6_INCLUDED
#define YACK_NWSORST_hibbard6_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard6
    class hibbard6 : public swaps {
      public:
        virtual ~hibbard6() throw(); //!< cleanup
        explicit hibbard6() throw(); //!< setup  
        static const char   sid[];        //!< hibbard6
        static const size_t lhs[0x000c];  //!< lhs
        static const size_t rhs[0x000c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard6);
    };
  }
}
#endif
