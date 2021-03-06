#include "yack/ptr/auto.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/ptr/cstr.hpp"
#include "yack/ptr/linked.hpp"
#include "yack/counted.hpp"

#include "yack/data/list/cxx.hpp"

#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
using namespace yack;

namespace
{
    class dummy : public object, public counted
    {
    public:
        int value;
        
        virtual ~dummy() throw() {}
        explicit dummy(const int a) throw() : value(a) {}
        
        inline friend std::ostream & operator<<(std::ostream &os, const dummy &self)
        {
            std::cerr << "[" << self.value << "]";
            return os;
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };
}


YACK_UTEST(memory_ptr)
{
    randomized::rand_ ran;

    YACK_SIZEOF(auto_ptr<dummy>);
    YACK_SIZEOF(arc_ptr<dummy>);
    YACK_SIZEOF(shared_ptr<dummy>);
    YACK_SIZEOF(counted_ptr<dummy>);
    YACK_SIZEOF(linked_ptr<dummy>);

    std::cerr << "NIL" << std::endl;
    std::cerr << ptr_::nil << std::endl;

    std::cerr << "auto_ptr" << std::endl;
    {
        auto_ptr<apq> nil;
        std::cerr << nil << std::endl;
    }
    
    {
        auto_ptr<apq> pq = new apq(1,2);
        std::cerr << pq << std::endl;
        pq = NULL;
        std::cerr << pq << std::endl;
        pq = new apq(2,3);
        std::cerr << pq << std::endl;
    }

    std::cerr << "arc_ptr" << std::endl;
    {
        arc_ptr<dummy> p = new dummy(2);
        std::cerr << p << std::endl;
        {
            arc_ptr<dummy> q = p;
            ++(q->value);
            std::cerr << q << std::endl;
        }
        std::cerr << p << std::endl;
    }

    std::cerr << "shared_ptr" << std::endl;
    {
        shared_ptr<apq> p = new apq(3,5);
        std::cerr << p << std::endl;
        {
            shared_ptr<apq> q = p;
            std::cerr << q->to_double() << std::endl;
            ++(*q);
            std::cerr << q->to_double() << std::endl;
        }
        std::cerr << p << std::endl;
    }


    std::cerr << "cstr_ptr" << std::endl;
    {
        cstr_ptr p(5);
        std::cerr << "p=" << p << std::endl;
    }
    {
        cstr_ptr p = "Hello, World!";
        std::cerr << "p=" << p << std::endl;
    }


    std::cerr << "linked_ptr" << std::endl;
    {
        typedef linked_ptr<dummy> node_type;
        cxx_list_of< node_type > L;
        for(size_t i=0;i<10;++i)
        {
            if( ran.choice() )
            {
                L.push_back( node_type::make( new dummy(i) ) );
            }
            else
            {
                L.push_front( node_type::make( new dummy(i) ) );
            }
        }
        for(const node_type *node=L.head;node;node=node->next)
        {
            std::cerr << **node << std::endl;
        }
    }
}
YACK_UDONE()

