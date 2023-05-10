#include "yack/data/clist/cxx.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


namespace
{

    class XNode : public object
    {
    public:
        XNode *next;
        XNode *prev;
        int    indx;

        inline  XNode(const int data) noexcept : object(), next(0), prev(0), indx(data)
        {
            std::cerr << "+Xnode(" << data << ")" << std::endl;
        }
        inline ~XNode() noexcept { assert(0==next); assert(0==prev); }
        inline  XNode(const XNode &other) noexcept : object(), next(0), prev(0), indx(other.indx) {}
         XNode *clone() const { return new XNode(*this); }

        inline friend std::ostream & operator<<(std::ostream &os, const XNode &node)
        {
            os << node.indx;
            return os;
        }
    private:
        YACK_DISABLE_ASSIGN(XNode);
    };

}

YACK_UTEST(data_clist)
{
    cxx_clist_of<XNode> xlist;
    randomized::rand_    ran;

    std::cerr << "push_back" << std::endl;
    for(int i=1;i<=10;++i)
    {
        xlist.push_back( new XNode(i) );
        std::cerr << xlist << std::endl;
    }
    while(xlist.size)
    {
        delete xlist.pop_back();
        std::cerr << xlist << std::endl;
    }

    std::cerr << "push_front" << std::endl;
    for(int i=1;i<=10;++i)
    {
        xlist.push_front( new XNode(i) );
        std::cerr << xlist << std::endl;
    }

    while(xlist.size)
    {
        delete xlist.pop_front();
        std::cerr << xlist << std::endl;
    }

    std::cerr << "reverse" << std::endl;
    for(int i=1;i<=10;++i)
    {
        if( ran.choice() ) xlist.push_front( new XNode(i) ); else xlist.push_back( new XNode(i) );
    }
    std::cerr << xlist << std::endl;
    {
        cxx_clist_of<XNode> ylist(xlist);
        ylist.reverse();
        std::cerr << ylist << std::endl;
    }

    xlist.release();
    for(int i=1;i<=20;++i)
    {
        xlist.push_back( new XNode(i) );
        std::cerr << xlist << std::endl;
        const XNode *node = xlist.head;
        for(size_t j=1;j<=xlist.size;++j,node=node->next)
        {
            const XNode *find = xlist.get(j);
            std::cerr << j << " -> " << *find << " / " << *node << std::endl;
            YACK_ASSERT(find->indx==node->indx);
        }
    }


}
YACK_UDONE()


