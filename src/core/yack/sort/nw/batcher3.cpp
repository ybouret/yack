#include "batcher3.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher3 :: sid[] ="batcher3";
    batcher3 :: ~batcher3() throw() {}
    batcher3 ::  batcher3() throw() : swaps(sid,3,0x0003,lhs,rhs) {}
    const size_t batcher3 :: lhs[0x0003] = {
 0x0001, 0x0001, 0x0002
    };
    const size_t batcher3 :: rhs[0x0003] = {
 0x0003, 0x0002, 0x0003
    };
  }
}
