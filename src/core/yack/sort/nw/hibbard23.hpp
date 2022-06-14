//! \file
#ifndef YACK_NWSORST_hibbard23_INCLUDED
#define YACK_NWSORST_hibbard23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard23
    class hibbard23 : public swaps {
      public:
        virtual ~hibbard23() throw(); //!< cleanup
        explicit hibbard23() throw(); //!< setup  
        static const char   sid[];        //!< hibbard23
        static const size_t lhs[0x0085];  //!< lhs
        static const size_t rhs[0x0085];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard23);
    };
  }
}
#endif
