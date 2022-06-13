//! \file
#ifndef YACK_NWSORST_hibbard26_INCLUDED
#define YACK_NWSORST_hibbard26_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard26
    class hibbard26 : public swaps {
      public:
        virtual ~hibbard26() throw(); //!< cleanup
        explicit hibbard26() throw(); //!< setup  
        static const char   sid[];        //!< hibbard26
        static const size_t lhs[0x00a3];  //!< lhs
        static const size_t rhs[0x00a3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard26);
    };
  }
}
#endif
