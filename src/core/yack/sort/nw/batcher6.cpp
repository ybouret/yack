#include "batcher6.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher6 :: sid[] ="batcher6";
    batcher6 :: ~batcher6() noexcept {}
    batcher6 ::  batcher6() noexcept : swaps(sid,6,0x000c,lhs,rhs) {}
    const size_t batcher6 :: lhs[0x000c] = {
 0x0001, 0x0002, 0x0001, 0x0002, 0x0003, 0x0004, 0x0001, 0x0003,
 0x0005, 0x0002, 0x0002, 0x0004
    };
    const size_t batcher6 :: rhs[0x000c] = {
 0x0005, 0x0006, 0x0003, 0x0004, 0x0005, 0x0006, 0x0002, 0x0004,
 0x0006, 0x0005, 0x0003, 0x0005
    };
  }
}
