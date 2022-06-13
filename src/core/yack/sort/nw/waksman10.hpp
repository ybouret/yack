//! \file
#ifndef YACK_NWSORST_waksman10_INCLUDED
#define YACK_NWSORST_waksman10_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! waksman10
    class waksman10 : public pairs<10,0x001d> {
      public:
        virtual ~waksman10() throw(); //!< cleanup
        explicit waksman10() throw(); //!< setup  
        static const char   sid[];        //!< waksman10
        static const size_t lhs[0x001d];  //!< lhs
        static const size_t rhs[0x001d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(waksman10);
    };
  }
}
#endif
