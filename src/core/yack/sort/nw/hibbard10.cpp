#include "hibbard10.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard10 :: sid[] ="hibbard10";
    hibbard10 :: ~hibbard10() throw() {}
    hibbard10 ::  hibbard10() throw() : swaps(sid,10,0x0020,lhs,rhs) {}
    const size_t hibbard10 :: lhs[0x0020] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x0001, 0x0002, 0x0005,
 0x0006, 0x0002, 0x0006, 0x0001, 0x0004, 0x0002, 0x0003, 0x0001,
 0x0002, 0x0004, 0x0003, 0x0004, 0x0002, 0x0004, 0x0002, 0x0003,
 0x0004, 0x0004, 0x0006, 0x0005, 0x0008, 0x0006, 0x0007, 0x0008
    };
    const size_t hibbard10 :: rhs[0x0020] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x0003, 0x0004, 0x0007,
 0x0008, 0x0003, 0x0007, 0x0005, 0x0008, 0x0006, 0x0007, 0x0009,
 0x0005, 0x0007, 0x0005, 0x0006, 0x000a, 0x0005, 0x0009, 0x0009,
 0x000a, 0x0009, 0x000a, 0x0009, 0x000a, 0x0009, 0x0009, 0x0009
    };
  }
}
