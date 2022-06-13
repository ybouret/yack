#include "waksman10.hpp"
namespace yack {
  namespace nwsrt {
    const char waksman10 :: sid[] ="waksman10";
    waksman10 :: ~waksman10() throw() {}
    waksman10 ::  waksman10() throw() : pairs<10,0x001d>(sid,lhs,rhs) {}
    const size_t waksman10 :: lhs[0x001d] = {
 0x0005, 0x0004, 0x0003, 0x0002, 0x0001, 0x0002, 0x0007, 0x0001,
 0x0006, 0x0001, 0x0004, 0x0008, 0x0001, 0x0003, 0x0006, 0x0009,
 0x0002, 0x0005, 0x0008, 0x0004, 0x0003, 0x0007, 0x0002, 0x0005,
 0x0003, 0x0007, 0x0004, 0x0006, 0x0005
    };
    const size_t waksman10 :: rhs[0x001d] = {
 0x000a, 0x0009, 0x0008, 0x0007, 0x0006, 0x0005, 0x000a, 0x0004,
 0x0009, 0x0003, 0x0007, 0x000a, 0x0002, 0x0005, 0x0008, 0x000a,
 0x0003, 0x0007, 0x0009, 0x0006, 0x0006, 0x0009, 0x0004, 0x0008,
 0x0004, 0x0008, 0x0005, 0x0007, 0x0006
    };
  }
}
