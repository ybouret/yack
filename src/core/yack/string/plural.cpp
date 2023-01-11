
#include "yack/string/plural.hpp"

namespace yack
{
    const char *plural:: s(const size_t n) throw() { return (n>1) ? "s" : ""; }
    const char *plural:: x(const size_t n) throw() { return (n>1) ? "x" : ""; }
    const char *plural:: a(const size_t n) throw() { return (n>1) ? "a" : "um"; }
}
