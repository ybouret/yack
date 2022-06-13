//! \file
#ifndef YACK_NWSORST_vanvoorhis16_INCLUDED
#define YACK_NWSORST_vanvoorhis16_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! vanvoorhis16
    class vanvoorhis16 : public pairs<16,0x003d> {
      public:
        virtual ~vanvoorhis16() throw(); //!< cleanup
        explicit vanvoorhis16() throw(); //!< setup  
        static const char   sid[];        //!< vanvoorhis16
        static const size_t lhs[0x003d];  //!< lhs
        static const size_t rhs[0x003d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(vanvoorhis16);
    };
  }
}
#endif
