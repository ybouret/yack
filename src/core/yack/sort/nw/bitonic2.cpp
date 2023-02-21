#include "bitonic2.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic2 :: sid[] ="bitonic2";
    bitonic2 :: ~bitonic2() noexcept {}
    bitonic2 ::  bitonic2() noexcept : swaps(sid,2,0x0001,lhs,rhs) {}
    const size_t bitonic2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t bitonic2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
