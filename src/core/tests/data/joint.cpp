#include "yack/data/list/com.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace {

    template <typename T>
    class xNode
    {
    public:
        YACK_DECL_ARGS(T,type);

        inline  xNode() : next(0), prev(0), data() {}
        inline ~xNode() throw() {}
        inline  xNode(param_type args)  : next(0), prev(0), data(args) {}

        xNode *next;
        xNode *prev;
        type   data;

        inline friend std::ostream & operator<<(std::ostream &os, const xNode &self)
        {
            os << self.data;
            return os;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(xNode);
    };

    typedef joint_pool<xNode<int> > xPool;
    typedef joint_list<xNode<int> > xList;


    typedef joint_pool<xNode<string> > sPool;
    typedef joint_list<xNode<string> > sList;
}

YACK_UTEST(data_joint)
{
    randomized::rand_ ran;

    YACK_SIZEOF(xPool);
    YACK_SIZEOF(xList);
    YACK_SIZEOF(xList::node_type);

    YACK_SIZEOF(sPool);
    YACK_SIZEOF(sList);
    YACK_SIZEOF(sList::node_type);

    {
        xPool::pointer fund = new xPool();

        fund->reserve(2);

        {
            xList l(fund);

            for(size_t i=10;i>0;--i)
            {
                if( ran.choice() )
                    l.push_back( fund->create()  );
                else
                    l.push_front( fund->create() );
            }

            l << default_type;
            l << 3;
            l >> default_type;
            l >> 4;
        }

        std::cerr << "@cache: " << (**fund).size << std::endl;
    }


    {
        sPool::pointer fund = new sPool();
        fund->reserve(4);

        {
            sList l(fund);

            l << "Hello";
            l << "World";
            l << default_type;
            std::cerr << "#strings=" << l.size << std::endl;

            std::cerr << l << std::endl;
        }
    }

    {
        const int tab[8] = {1,2,3,4,5,6,7,8};

        com_pool<const int *>::pointer fund = new com_pool<const int *>();
        com_list<const int *>          L(fund);
        for(size_t i=0;i<sizeof(tab)/sizeof(tab[0]);++i)
        {
            if(ran.choice()) L << &coerce(tab[i]); else L >> &coerce(tab[i]);
            //L << tab+i;
        }
        std::cerr << L << std::endl;
    }


}
YACK_UDONE()

