
#include "yack/jive/pattern/logic/bank.hpp"
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/jive/pattern/basic/all.hpp"

namespace yack
{
    namespace jive
    {

        bank:: ~bank() noexcept
        {
        }

        bank:: bank(const uint32_t t) noexcept : logical(t)
        {

        }

        bank:: bank(const bank &other) :
        logical(other)
        {
        }
        

        void bank:: no_multiple() noexcept
        {
            patterns temp;
            while(size)
            {
                bool found = false;
                for(const pattern *m=temp.head;m;m=m->next)
                {
                    if( *m == *head )
                    {
                        found = true;
                        break;
                    }
                }
                pattern *mine = pop_front();
                if(found)
                {
                    delete mine;
                }
                else
                {
                    temp.push_back(mine);
                }
            }
            swap_with(temp);
        }


        void bank:: smart_merge()  
        {
            patterns    hl; //!< high level
            first_bytes fc; //!< low  level
            while(size)
            {
                switch(head->uuid)
                {
                    case single::mark:
                        fc << new domain( head->as<single>()->code );
                        delete pop_front();
                        break;

                    case within::mark: {
                        const within &p = * (head->as<within>());
                        fc << new domain(p.lower,p.upper);
                        delete pop_front();
                    } break;

                    default:
                        hl.push_back(pop_front());
                }
            }
            assert(0==size);

            fc.build(*this); // append singulet/interval
            merge_back(hl);  // append higher level

        }

        void bank::rewrite()
        {
            no_multiple();
            smart_merge();
        }


    }

}

