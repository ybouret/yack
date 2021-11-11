#include "yack/ptr/auto.hpp"
#include "yack/ptr/arc.hpp"
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
    std::cerr << ptr_::nil << std::endl;
    
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
    
    {
        arc_ptr<dummy> p = new dummy(2);
        std::cerr << p << std::endl;
        
    }
}
YACK_UDONE()

