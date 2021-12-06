
#include "yack/jive/pattern/logic/bank.hpp"
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/jive/pattern/basic/all.hpp"

namespace yack
{
    namespace jive
    {

        bank:: ~bank() throw()
        {
        }

        bank:: bank(const uint32_t t) throw() : logical(t)
        {

        }

        bank:: bank(const bank &other) :
        logical(other)
        {
        }
        

        void bank:: no_multiple() throw()
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
            patterns    si; //!< singulet
            patterns    in; //!< interval
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
            for(const domain *dom = fc.head; dom; dom=dom->next )
            {
                if(dom->lower>=dom->upper)
                {
                    si.push_back( new single(dom->lower) );
                }
                else
                {
                    in.push_back( new within(dom->lower,dom->upper) );
                }
            }
            merge_back(si);
            merge_back(in);
            merge_back(hl);

        }

        void bank::rewrite()
        {
            no_multiple();
            smart_merge();
        }


    }

}

