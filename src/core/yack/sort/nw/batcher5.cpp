#include "batcher5.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher5 :: sid[] ="batcher5";
    batcher5 :: ~batcher5() noexcept {}
    batcher5 ::  batcher5() noexcept : swaps(sid,5,0x0009,lhs,rhs) {}
    const size_t batcher5 :: lhs[0x0009] = {
 0x0001, 0x0002, 0x0001, 0x0003, 0x0001, 0x0003, 0x0002, 0x0002,
 0x0004
    };
    const size_t batcher5 :: rhs[0x0009] = {
 0x0005, 0x0004, 0x0003, 0x0005, 0x0002, 0x0004, 0x0005, 0x0003,
 0x0005
    };
  }
}
