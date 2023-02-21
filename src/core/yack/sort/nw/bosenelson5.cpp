#include "bosenelson5.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson5 :: sid[] ="bosenelson5";
    bosenelson5 :: ~bosenelson5() noexcept {}
    bosenelson5 ::  bosenelson5() noexcept : swaps(sid,5,0x0009,lhs,rhs) {}
    const size_t bosenelson5 :: lhs[0x0009] = {
 0x0001, 0x0004, 0x0003, 0x0003, 0x0002, 0x0001, 0x0001, 0x0002,
 0x0002
    };
    const size_t bosenelson5 :: rhs[0x0009] = {
 0x0002, 0x0005, 0x0005, 0x0004, 0x0005, 0x0004, 0x0003, 0x0004,
 0x0003
    };
  }
}
