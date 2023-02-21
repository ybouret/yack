#include "hibbard6.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard6 :: sid[] ="hibbard6";
    hibbard6 :: ~hibbard6() noexcept {}
    hibbard6 ::  hibbard6() noexcept : swaps(sid,6,0x000c,lhs,rhs) {}
    const size_t hibbard6 :: lhs[0x000c] = {
 0x0001, 0x0003, 0x0005, 0x0001, 0x0002, 0x0002, 0x0001, 0x0002,
 0x0002, 0x0004, 0x0003, 0x0004
    };
    const size_t hibbard6 :: rhs[0x000c] = {
 0x0002, 0x0004, 0x0006, 0x0003, 0x0004, 0x0003, 0x0005, 0x0006,
 0x0005, 0x0006, 0x0005, 0x0005
    };
  }
}
