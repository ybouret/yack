#include "batcher14.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher14 :: sid[] ="batcher14";
    batcher14 :: ~batcher14() throw() {}
    batcher14 ::  batcher14() throw() : swaps(sid,14,0x0035,lhs,rhs) {}
    const size_t batcher14 :: lhs[0x0035] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0001, 0x0002,
 0x0003, 0x0004, 0x0009, 0x000a, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0001, 0x0002, 0x0005, 0x0006, 0x0009, 0x000a, 0x0001, 0x0003,
 0x0004, 0x0007, 0x0008, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b,
 0x000c, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x0002,
 0x0004, 0x0006, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x0002,
 0x0004, 0x0006, 0x0008, 0x000a, 0x000c
    };
    const size_t batcher14 :: rhs[0x0035] = {
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x0005, 0x0006,
 0x0007, 0x0008, 0x000d, 0x000e, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x0002, 0x0009,
 0x000a, 0x000d, 0x000e, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d,
 0x000e, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0009,
 0x000b, 0x000d, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x0003,
 0x0005, 0x0007, 0x0009, 0x000b, 0x000d
    };
  }
}
