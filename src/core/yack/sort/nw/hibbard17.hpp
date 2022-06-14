//! \file
#ifndef YACK_NWSORST_hibbard17_INCLUDED
#define YACK_NWSORST_hibbard17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard17
    class hibbard17 : public swaps {
      public:
        virtual ~hibbard17() throw(); //!< cleanup
        explicit hibbard17() throw(); //!< setup  
        static const char   sid[];        //!< hibbard17
        static const size_t lhs[0x0051];  //!< lhs
        static const size_t rhs[0x0051];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard17);
    };
  }
}
#endif
