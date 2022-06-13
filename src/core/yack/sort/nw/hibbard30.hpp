//! \file
#ifndef YACK_NWSORST_hibbard30_INCLUDED
#define YACK_NWSORST_hibbard30_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard30
    class hibbard30 : public pairs<30,0x00c6> {
      public:
        virtual ~hibbard30() throw(); //!< cleanup
        explicit hibbard30() throw(); //!< setup  
        static const char   sid[];        //!< hibbard30
        static const size_t lhs[0x00c6];  //!< lhs
        static const size_t rhs[0x00c6];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard30);
    };
  }
}
#endif
