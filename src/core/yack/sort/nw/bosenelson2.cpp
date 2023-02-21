#include "bosenelson2.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson2 :: sid[] ="bosenelson2";
    bosenelson2 :: ~bosenelson2() noexcept {}
    bosenelson2 ::  bosenelson2() noexcept : swaps(sid,2,0x0001,lhs,rhs) {}
    const size_t bosenelson2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t bosenelson2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
