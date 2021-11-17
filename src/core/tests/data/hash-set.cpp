
#include "yack/associative/hash/set.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

namespace
{
    class dummy : public object
    {
    public:
        const int k;

        dummy(const int a) throw() : object(), k(a)
        {
        }

        dummy(const dummy &d) throw() : object(), k(d.k)
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
        vector<int> keys;
        for(int i=0;i<100;++i)
        {
            keys.push_back(i);
            const dummy  d(i);
            YACK_ASSERT(s.insert(d));
        }
    }
}
YACK_UDONE()
