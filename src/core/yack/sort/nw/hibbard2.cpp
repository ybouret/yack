#include "hibbard2.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard2 :: sid[] ="hibbard2";
    hibbard2 :: ~hibbard2() noexcept {}
    hibbard2 ::  hibbard2() noexcept : swaps(sid,2,0x0001,lhs,rhs) {}
    const size_t hibbard2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t hibbard2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
