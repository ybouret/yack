#include "green14.hpp"
namespace yack {
  namespace nwsrt {
    const char green14 :: sid[] ="green14";
    green14 :: ~green14() throw() {}
    green14 ::  green14() throw() : swaps(sid,14,0x0033,lhs,rhs) {}
    const size_t green14 :: lhs[0x0033] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x0001,
 0x0005, 0x0009, 0x0002, 0x0006, 0x000a, 0x0001, 0x0009, 0x0002,
 0x000a, 0x0003, 0x0004, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
 0x0006, 0x0006, 0x0007, 0x0004, 0x0008, 0x0002, 0x0005, 0x0002,
 0x0008, 0x0003, 0x0006, 0x000a, 0x0003, 0x000c, 0x0004, 0x0008,
 0x0007, 0x000b, 0x0004, 0x0008, 0x0004, 0x0006, 0x0008, 0x000a,
 0x000c, 0x0007, 0x0009
    };
    const size_t green14 :: rhs[0x0033] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0003,
 0x0007, 0x000b, 0x0004, 0x0008, 0x000c, 0x0005, 0x000d, 0x0006,
 0x000e, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d,
 0x000e, 0x000b, 0x000a, 0x000d, 0x000c, 0x0003, 0x0009, 0x0005,
 0x000e, 0x0009, 0x0007, 0x000b, 0x0005, 0x000e, 0x0009, 0x000d,
 0x0009, 0x000d, 0x0006, 0x000a, 0x0005, 0x0007, 0x0009, 0x000b,
 0x000d, 0x0008, 0x000a
    };
  }
}