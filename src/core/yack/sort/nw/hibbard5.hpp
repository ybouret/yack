//! \file
#ifndef YACK_NWSORST_hibbard5_INCLUDED
#define YACK_NWSORST_hibbard5_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard5
    class hibbard5 : public pairs<5,0x0009> {
      public:
        virtual ~hibbard5() throw(); //!< cleanup
        explicit hibbard5() throw(); //!< setup  
        static const char   sid[];        //!< hibbard5
        static const size_t lhs[0x0009];  //!< lhs
        static const size_t rhs[0x0009];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard5);
    };
  }
}
#endif
