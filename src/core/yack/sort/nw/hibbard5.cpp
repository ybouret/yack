#include "hibbard5.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard5 :: sid[] ="hibbard5";
    hibbard5 :: ~hibbard5() throw() {}
    hibbard5 ::  hibbard5() throw() : swaps(sid,5,0x0009,lhs,rhs) {}
    const size_t hibbard5 :: lhs[0x0009] = {
 0x0001, 0x0003, 0x0001, 0x0002, 0x0002, 0x0001, 0x0002, 0x0003,
 0x0004
    };
    const size_t hibbard5 :: rhs[0x0009] = {
 0x0002, 0x0004, 0x0003, 0x0004, 0x0003, 0x0005, 0x0005, 0x0005,
 0x0005
    };
  }
}
