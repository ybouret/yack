//! \file
#ifndef YACK_NWSORST_hibbard19_INCLUDED
#define YACK_NWSORST_hibbard19_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard19
    class hibbard19 : public swaps {
      public:
        virtual ~hibbard19() throw(); //!< cleanup
        explicit hibbard19() throw(); //!< setup  
        static const char   sid[];        //!< hibbard19
        static const size_t lhs[0x0064];  //!< lhs
        static const size_t rhs[0x0064];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard19);
    };
  }
}
#endif
