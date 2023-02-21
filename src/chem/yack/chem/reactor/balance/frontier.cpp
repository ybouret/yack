#include "yack/chem/reactor/balance/frontier.hpp"
#include <iomanip>

namespace yack {

    namespace chemical
    {

        frontier:: ~frontier() noexcept
        {
#if 64 == YACK_NATIVE_BITS
            coerce(unused) = 0;
#endif
        }

        frontier:: frontier(const sp_fund &fund) noexcept :
        sp_para(fund),
        xi(0)
#if 64 == YACK_NATIVE_BITS
        , unused(0)
#endif
        {}

        frontier:: frontier(const frontier &other) :
        sp_para(other),
        xi(0)
#if 64 == YACK_NATIVE_BITS
        , unused(0)
#endif
        {
        }
        

        void frontier:: free() noexcept
        {
            xi = 0;
            clear();
        }

        frontier & frontier:: operator=(const frontier &other)
        {
            sp_para temp(other);
            swap_with(temp);
            xi = other.xi;
            return *this;
        }


        void frontier:: operator()(const double xx, const species &s) {
            assert(xx>=0);

            if(size<=0) {
                xi = xx;
                (*this) << s;
            }
            else
            {
                switch( __sign::of(xx,xi) )
                {
                    case negative:
                        xi = xx;
                        clear();
                        (*this) << s;
                        return;

                    case __zero__:
                        (*this) << s;
                        return;

                    case positive:
                        return;
                }
            }

        }

        void frontier:: vanish(writable<double> &C) const noexcept
        {
            for(const sp_node *sn=head;sn;sn=sn->next)
            {
                const species &s = ***sn;
                C[*s] = 0;
            }
        }

        bool frontier:: adjust(const readable<double> &C, const actors &A)
        {
            free();

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
