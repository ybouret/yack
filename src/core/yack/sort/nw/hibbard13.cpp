#include "hibbard13.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard13 :: sid[] ="hibbard13";
    hibbard13 :: ~hibbard13() noexcept {}
    hibbard13 ::  hibbard13() noexcept : swaps(sid,13,0x0032,lhs,rhs) {}
    const size_t hibbard13 :: lhs[0x0032] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x0001, 0x0002,
 0x0005, 0x0006, 0x0009, 0x000a, 0x0002, 0x0006, 0x0001, 0x0004,
 0x000a, 0x0009, 0x0002, 0x0003, 0x000a, 0x0001, 0x0002, 0x0004,
 0x000b, 0x0003, 0x0004, 0x000c, 0x0002, 0x0004, 0x0002, 0x0003,
 0x0004, 0x0003, 0x0005, 0x0004, 0x0006, 0x0004, 0x0007, 0x0004,
 0x0008, 0x0006, 0x0007, 0x0005, 0x0008, 0x0006, 0x0008, 0x0007,
 0x0008, 0x0008
    };
    const size_t hibbard13 :: rhs[0x0032] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x0003, 0x0004,
 0x0007, 0x0008, 0x000b, 0x000c, 0x0003, 0x0007, 0x0005, 0x0008,
 0x000b, 0x000d, 0x0006, 0x0007, 0x000d, 0x0009, 0x0005, 0x0007,
 0x000d, 0x0005, 0x0006, 0x000d, 0x000a, 0x0005, 0x0009, 0x000b,
 0x000c, 0x0009, 0x000d, 0x000b, 0x000d, 0x000a, 0x000d, 0x0009,
 0x000d, 0x000a, 0x000b, 0x0009, 0x000c, 0x0009, 0x000b, 0x0009,
 0x000a, 0x0009
    };
  }
}
