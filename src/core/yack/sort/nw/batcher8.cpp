#include "batcher8.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher8 :: sid[] ="batcher8";
    batcher8 :: ~batcher8() throw() {}
    batcher8 ::  batcher8() throw() : swaps(sid,8,0x0013,lhs,rhs) {}
    const size_t batcher8 :: lhs[0x0013] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0001, 0x0002, 0x0005, 0x0006,
 0x0003, 0x0004, 0x0001, 0x0007, 0x0003, 0x0005, 0x0002, 0x0004,
 0x0002, 0x0004, 0x0006
    };
    const size_t batcher8 :: rhs[0x0013] = {
 0x0005, 0x0006, 0x0007, 0x0008, 0x0003, 0x0004, 0x0007, 0x0008,
 0x0005, 0x0006, 0x0002, 0x0008, 0x0004, 0x0006, 0x0005, 0x0007,
 0x0003, 0x0005, 0x0007
    };
  }
}
