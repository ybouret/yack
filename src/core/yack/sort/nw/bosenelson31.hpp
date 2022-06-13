//! \file
#ifndef YACK_NWSORST_bosenelson31_INCLUDED
#define YACK_NWSORST_bosenelson31_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson31
    class bosenelson31 : public pairs<31,0x00ce> {
      public:
        virtual ~bosenelson31() throw(); //!< cleanup
        explicit bosenelson31() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson31
        static const size_t lhs[0x00ce];  //!< lhs
        static const size_t rhs[0x00ce];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson31);
    };
  }
}
#endif
