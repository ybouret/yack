//! \file
#ifndef YACK_NWSORST_end13_INCLUDED
#define YACK_NWSORST_end13_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! end13
    class end13 : public pairs<13,0x002d> {
      public:
        virtual ~end13() throw(); //!< cleanup
        explicit end13() throw(); //!< setup  
        static const char   sid[];        //!< end13
        static const size_t lhs[0x002d];  //!< lhs
        static const size_t rhs[0x002d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(end13);
    };
  }
}
#endif
