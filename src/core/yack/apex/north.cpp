#include "yack/apex/north.hpp"
#include "yack/system/imported.hpp"
#include "yack/memory/allocator/global.hpp"

namespace yack
{

    namespace north
    {

        static const char here[] = "north";
        static const char mesg[] = "null dimension!";

        size_t constellation:: checked_dimension(const size_t d)
        {
            if(d<=0) throw imported::exception(here,mesg);
            return d;
        }

        maturity constellation:: initial_situation(const size_t dims)
        {
            switch(dims)
            {
                case 0: throw  imported::exception(here,mesg);
                case 1: return fully_grown;
                case 2: return almost_done;
                default:
                    break;
            }
            return in_progress;
        }

        maturity constellation:: updated_situation(const size_t dims, const size_t size) throw()
        {
            assert(dims>0);
            if(size>=dims)
            {
                assert(dims==size);
                return fully_grown;
            }
            else
            {
                const size_t mark = dims-1;
                if(size<mark)
                {
                    return  in_progress;
                }
                else
                {
                    assert(dims-1==size);
                    return  almost_done;
                }
            }
        }

    }

}
