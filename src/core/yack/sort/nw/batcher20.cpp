#include "batcher20.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher20 :: sid[] ="batcher20";
    batcher20 :: ~batcher20() throw() {}
    batcher20 ::  batcher20() throw() : swaps(sid,20,0x0061,lhs,rhs) {}
    const size_t batcher20 :: lhs[0x0061] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0005, 0x0006, 0x0007, 0x0008, 0x0001, 0x0002, 0x0005, 0x0006,
 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010, 0x0001, 0x0005,
 0x0006, 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012, 0x0003,
 0x0004, 0x0007, 0x0008, 0x0013, 0x0003, 0x0004, 0x000b, 0x000c,
 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010,
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011,
 0x0002, 0x0004, 0x0006, 0x0008, 0x0002, 0x0004, 0x000a, 0x000c,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012
    };
    const size_t batcher20 :: rhs[0x0061] = {
 0x0011, 0x0012, 0x0013, 0x0014, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0009, 0x000a, 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014,
 0x0005, 0x0006, 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0003, 0x0004, 0x0009, 0x000a,
 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014, 0x0002, 0x0007,
 0x0008, 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014, 0x0011,
 0x0012, 0x000d, 0x000e, 0x0014, 0x0009, 0x000a, 0x0011, 0x0012,
 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012,
 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012,
 0x0011, 0x0013, 0x000d, 0x000f, 0x0009, 0x000b, 0x0011, 0x0013,
 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0013,
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011,
 0x0013
    };
  }
}
