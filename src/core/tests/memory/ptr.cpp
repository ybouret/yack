#include "yack/ptr/auto.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/counted.hpp"

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
    YACK_SIZEOF(auto_ptr<dummy>);
    YACK_SIZEOF(arc_ptr<dummy>);
    YACK_SIZEOF(shared_ptr<dummy>);
    YACK_SIZEOF(counted_ptr<dummy>);

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
    }

}
YACK_UDONE()

