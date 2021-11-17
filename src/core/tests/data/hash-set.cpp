
#include "yack/associative/hash/set.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class dummy : public object
    {
    public:
        const int k;

        dummy(const int a) throw() : k(a)
        {
        }

        dummy(const dummy &d) throw() : k(d.k)
        {
        }


        ~dummy() throw()
        {
        }

        const int & key() const throw() { return k; }

    private:
        YACK_DISABLE_ASSIGN(dummy);
    };
}

YACK_UTEST(data_hash_set)
{
    {
        hash_set<int,dummy> s;

        s.search(5);
    }
}
YACK_UDONE()
