//! \file
#ifndef YACK_NWSORST_hibbard7_INCLUDED
#define YACK_NWSORST_hibbard7_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard7
    class hibbard7 : public pairs<7,0x0010> {
      public:
        virtual ~hibbard7() throw(); //!< cleanup
        explicit hibbard7() throw(); //!< setup  
        static const char   sid[];        //!< hibbard7
        static const size_t lhs[0x0010];  //!< lhs
        static const size_t rhs[0x0010];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard7);
    };
  }
}
#endif
