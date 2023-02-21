#include "batcher2.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher2 :: sid[] ="batcher2";
    batcher2 :: ~batcher2() noexcept {}
    batcher2 ::  batcher2() noexcept : swaps(sid,2,0x0001,lhs,rhs) {}
    const size_t batcher2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t batcher2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
