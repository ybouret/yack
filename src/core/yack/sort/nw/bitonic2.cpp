#include "bitonic2.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic2 :: sid[] ="bitonic2";
    bitonic2 :: ~bitonic2() throw() {}
    bitonic2 ::  bitonic2() throw() : pairs<2,0x0001>(sid,lhs,rhs) {}
    const size_t bitonic2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t bitonic2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
