#include "bitonic12.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic12 :: sid[] ="bitonic12";
    bitonic12 :: ~bitonic12() throw() {}
    bitonic12 ::  bitonic12() throw() : swaps(sid,12,0x002e,lhs,rhs) {}
    const size_t bitonic12 :: lhs[0x002e] = {
 0x0002, 0x0005, 0x0008, 0x000b, 0x0001, 0x0004, 0x0007, 0x000a,
 0x0001, 0x0005, 0x0008, 0x000a, 0x0001, 0x0002, 0x0009, 0x0008,
 0x0003, 0x0004, 0x0001, 0x0007, 0x0008, 0x000b, 0x0005, 0x0003,
 0x0007, 0x0009, 0x0006, 0x0005, 0x0004, 0x0003, 0x0002, 0x0001,
 0x0004, 0x0003, 0x0009, 0x000a, 0x0002, 0x0001, 0x0006, 0x0005,
 0x0009, 0x000b, 0x0001, 0x0003, 0x0005, 0x0007
    };
    const size_t bitonic12 :: rhs[0x002e] = {
 0x0003, 0x0006, 0x0009, 0x000c, 0x0003, 0x0005, 0x0008, 0x000c,
 0x0002, 0x0006, 0x0009, 0x000b, 0x0005, 0x0004, 0x000b, 0x000c,
 0x0005, 0x0006, 0x0002, 0x0009, 0x000a, 0x000c, 0x0006, 0x0004,
 0x0008, 0x000a, 0x0009, 0x000a, 0x000b, 0x000c, 0x0006, 0x0005,
 0x0007, 0x0008, 0x000b, 0x000c, 0x0004, 0x0003, 0x0007, 0x0008,
 0x000a, 0x000c, 0x0002, 0x0004, 0x0006, 0x0008
    };
  }
}
