#include "batcher11.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher11 :: sid[] ="batcher11";
    batcher11 :: ~batcher11() noexcept {}
    batcher11 ::  batcher11() noexcept : swaps(sid,11,0x0025,lhs,rhs) {}
    const size_t batcher11 :: lhs[0x0025] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0001, 0x0002, 0x0003, 0x0005,
 0x0006, 0x0007, 0x0001, 0x0002, 0x0005, 0x0006, 0x0009, 0x0004,
 0x0001, 0x0003, 0x0004, 0x0008, 0x0003, 0x0007, 0x0003, 0x0005,
 0x0007, 0x0009, 0x0002, 0x0004, 0x0002, 0x0004, 0x0006, 0x0008,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a
    };
    const size_t batcher11 :: rhs[0x0025] = {
 0x0009, 0x000a, 0x000b, 0x0008, 0x0005, 0x0006, 0x0007, 0x0009,
 0x000a, 0x000b, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000a,
 0x0002, 0x0009, 0x0006, 0x000a, 0x0005, 0x0009, 0x0004, 0x0006,
 0x0008, 0x000a, 0x0009, 0x000b, 0x0005, 0x0007, 0x0009, 0x000b,
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b
    };
  }
}
