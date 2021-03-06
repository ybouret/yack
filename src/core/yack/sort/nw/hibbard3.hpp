//! \file
#ifndef YACK_NWSORST_hibbard3_INCLUDED
#define YACK_NWSORST_hibbard3_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard3
    class hibbard3 : public swaps {
      public:
        virtual ~hibbard3() throw(); //!< cleanup
        explicit hibbard3() throw(); //!< setup  
        static const char   sid[];        //!< hibbard3
        static const size_t lhs[0x0003];  //!< lhs
        static const size_t rhs[0x0003];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard3);
    };
  }
}
#endif
