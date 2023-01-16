#include "yack/chem/reactor/balance/frontier.hpp"
#include <iomanip>

namespace yack {

    namespace chemical
    {

        frontier:: ~frontier() throw()
        {
        }

        frontier:: frontier(const sp_fund &fund) throw() : sp_para(fund), xi(0) {}

        frontier:: frontier(const frontier &other) :
        sp_para(other),
        xi(0)
        {
        }
        


        void frontier:: remove() throw()
        {
            xi = 0;
            clear();
        }

        void frontier:: vanish(writable<double> &C) const throw()
        {
            for(const sp_node *sn=head;sn;sn=sn->next)
            {
                const species &s = ***sn;
                C[*s] = 0;
            }
        }


        bool frontier:: adjust(const readable<double> &C, const actors &A)
        {
            remove();

            //------------------------------------------------------------------
            // find fist negative value
            //------------------------------------------------------------------
            sp_para      &self = *this;
            const actor *node = A->head;
            for(;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   c = C[j];
                if(c<0) {
                    xi   = (-c)/node->nu;
                    self << s;
                    break;
                }
            }

            if(size<=0)
            {
                //--------------------------------------------------------------
                // no first negative value
                //--------------------------------------------------------------
                assert(NULL==node);
                return false;
            }
            else
            {
                //--------------------------------------------------------------
                // look up for greater value
                //--------------------------------------------------------------
                assert(node!=NULL);
                for(node=node->next;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   c = C[j];
                    if(c<0) {
                        const double xx = (-c)/node->nu;
                        switch( __sign::of(xx,xi) )
                        {
                            case negative: assert(xx<xi); continue; // xi is gt xx
                            case __zero__: self <<  s; continue;    // same numeric xi
                            case positive: assert(xi<xx);           // xx wins over xi
                                self.clear();
                                xi = xx;
                                self << s;
                                continue;
                        }
                    }
                }
                assert(size>0);
                return true;
            }
        }

        std::ostream & operator<<(std::ostream &os, const frontier &f)
        {
            const sp_para &p = f;
            os << "@" << std::setw(15) << f.xi << "->" << p;
            return os;
        }

    }

}
