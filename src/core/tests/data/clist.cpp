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

        inline  XNode(const int data) noexcept : next(0), prev(0), indx(data)
        {
            std::cerr << "+Xnode(" << data << ")" << std::endl;
        }
        inline ~XNode() noexcept { assert(0==next); assert(0==prev); }
        inline  XNode(const XNode &other) noexcept : next(0), prev(0), indx(other.indx) {}
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
    xlist.reverse();
    std::cerr << xlist << std::endl;

}
YACK_UDONE()


