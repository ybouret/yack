#include "yack/data/hash/table.hpp"
#include "yack/hashing/to.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    template <typename KEY>
    class dummy : public object
    {
    public:
        dummy    *next;
        dummy    *prev;
        const KEY key_;

        inline dummy(const KEY &user_key) :
        next(0),
        prev(0),
        key_(user_key)
        {
        }

        inline virtual ~dummy() throw()
        {
        }

        inline const KEY & key() const throw()
        {
            return key_;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };



}

YACK_UTEST(data_hash)
{

    typedef dummy<int>    Dummy;
    hash_table<int,Dummy> tab;
    
    


}
YACK_UDONE()


