#include "hibbard8.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard8 :: sid[] ="hibbard8";
    hibbard8 :: ~hibbard8() throw() {}
    hibbard8 ::  hibbard8() throw() : swaps(sid,8,0x0013,lhs,rhs) {}
    const size_t hibbard8 :: lhs[0x0013] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0001, 0x0002, 0x0005, 0x0006,
 0x0002, 0x0006, 0x0001, 0x0004, 0x0002, 0x0003, 0x0002, 0x0004,
 0x0003, 0x0004, 0x0004
    };
    const size_t hibbard8 :: rhs[0x0013] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x0003, 0x0004, 0x0007, 0x0008,
 0x0003, 0x0007, 0x0005, 0x0008, 0x0006, 0x0007, 0x0005, 0x0007,
 0x0005, 0x0006, 0x0005
    };
  }
}
