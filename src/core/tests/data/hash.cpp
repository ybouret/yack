#include "yack/data/hash/table.hpp"
#include "yack/hashing/to.hpp"
#include "yack/utest/run.hpp"
#include "yack/hashing/md5.hpp"

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

        static dummy *Quit(dummy *d) throw() { return d; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };



}

YACK_UTEST(data_hash)
{


    typedef dummy<int>                     Dummy;
    typedef kernel::hash_table<int,Dummy>  DTable;
    DTable                         tab;
    hashing::to_hkey<hashing::md5> kh;
    cxx_pool_of<Dummy>             dpool;

    YACK_SIZEOF(Dummy);
    YACK_SIZEOF(DTable::meta_node);

    {
        const int    user = 0;
        const size_t hkey = kh(user); std::cerr << "hkey0=" << hkey << std::endl;
        YACK_CHECK( !tab.search(hkey,user) );
        Dummy *d  = new Dummy(user);
        YACK_CHECK( tab.insert(hkey,d->key(),d,dpool,Dummy::Quit) );
    }


    {
        for(int i=1;i<100;++i)
        {
            Dummy       *d  = new Dummy(i);
            const size_t h  = kh(d->key());
            YACK_ASSERT( !tab.search(h,d->key()));
            YACK_ASSERT( tab.insert(h,d->key(),d,dpool,Dummy::Quit) );
        }
    }

    std::cerr << "size= " << tab.size() << std::endl;
    std::cerr << "load= " << tab.average_load() << std::endl;

    const size_t x2 = tab.exp2_for(4);
    std::cerr << "-> exp2=" << x2 << std::endl;

    tab.reload(x2);
    std::cerr << "load= " << tab.average_load() << std::endl;


    tab.free_with(dpool,Dummy::Quit);
    std::cerr << "size= " << tab.size() << std::endl;
    
    
}
YACK_UDONE()


