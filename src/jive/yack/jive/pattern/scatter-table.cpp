
#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include <new>


namespace yack
{
    namespace jive
    {

        scatter_node:: scatter_node(const pattern &p) throw() :
        next(0),
        prev(0),
        host(p)
        {

        }

        scatter_node:: ~scatter_node() throw()
        {

        }


    }

}

namespace yack
{
    namespace jive
    {
        scatter_table:: scatter_table():
        slot( static_cast<slot_type *>(memory::dyadic::instance().query(work_exp2)) )
        {
            slot_type *s = slot;
            for(size_t i=0;i<num_codes;++i)
            {
                new (s++) slot_type();
            }
        }

        scatter_table:: ~scatter_table() throw()
        {
            slot_type *s = slot;
            for(size_t i=0;i<num_codes;++i)
            {
                while(s->size) delete s->pop_back();
                out_of_reach::zset(destructed(s),slot_size);
                ++s;
            }
            memory::dyadic::location().store(slot,work_exp2);
        }

        const scatter_table::slot_type & scatter_table:: operator[](const uint8_t code) const throw()
        {
            return slot[code];
        }


    }

}

#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        void scatter_table:: operator()(const pattern &p)
        {
            first_bytes fc;
            p.firsts(fc);
            for(const domain *dom=fc.head;dom;dom=dom->next)
            {
                const size_t up = dom->upper;
                for(size_t code=dom->lower;code<=up;++code)
                {
                    assert(code<num_codes);
                    slot[code].push_back( new node_type(p) );
                }
            }
        }

    }

}

