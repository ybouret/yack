#include "hibbard2.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard2 :: sid[] ="hibbard2";
    hibbard2 :: ~hibbard2() throw() {}
    hibbard2 ::  hibbard2() throw() : pairs<2,0x0001>(sid,lhs,rhs) {}
    const size_t hibbard2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t hibbard2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
