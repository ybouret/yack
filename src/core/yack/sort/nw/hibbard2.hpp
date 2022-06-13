//! \file
#ifndef YACK_NWSORST_hibbard2_INCLUDED
#define YACK_NWSORST_hibbard2_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard2
    class hibbard2 : public pairs<2,0x0001> {
      public:
        virtual ~hibbard2() throw(); //!< cleanup
        explicit hibbard2() throw(); //!< setup  
        static const char   sid[];        //!< hibbard2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard2);
    };
  }
}
#endif
