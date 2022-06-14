//! \file
#ifndef YACK_NWSORST_hibbard15_INCLUDED
#define YACK_NWSORST_hibbard15_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard15
    class hibbard15 : public swaps {
      public:
        virtual ~hibbard15() throw(); //!< cleanup
        explicit hibbard15() throw(); //!< setup  
        static const char   sid[];        //!< hibbard15
        static const size_t lhs[0x003d];  //!< lhs
        static const size_t rhs[0x003d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard15);
    };
  }
}
#endif
