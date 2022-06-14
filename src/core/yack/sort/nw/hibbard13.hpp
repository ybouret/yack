//! \file
#ifndef YACK_NWSORST_hibbard13_INCLUDED
#define YACK_NWSORST_hibbard13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard13
    class hibbard13 : public swaps {
      public:
        virtual ~hibbard13() throw(); //!< cleanup
        explicit hibbard13() throw(); //!< setup  
        static const char   sid[];        //!< hibbard13
        static const size_t lhs[0x0032];  //!< lhs
        static const size_t rhs[0x0032];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard13);
    };
  }
}
#endif
