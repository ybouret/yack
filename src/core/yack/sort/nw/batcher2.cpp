#include "batcher2.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher2 :: sid[] ="batcher2";
    batcher2 :: ~batcher2() throw() {}
    batcher2 ::  batcher2() throw() : pairs<2,0x0001>(sid,lhs,rhs) {}
    const size_t batcher2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t batcher2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
