#include "yack/chem/reactor/conservation/law.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        claw::  claw(const size_t i) throw() :
        object(),
        indexed(i),
        next(0),
        prev(0),
        nrm2(0),
        crew()
        {}
        claw:: ~claw() throw() {}


        void claw::add(const species  &sp,
                       const unsigned  cf)
        {
            crew.push_back( new actor(sp,cf) );
        }

        const list_of<actor> * claw:: operator->() const throw() { return &crew; }

        std::ostream & operator<<(std::ostream &os, const claw &self)
        {
            os << '(';
            assert(self->size>=2);
            const actor *a = self->head;
            a->display_first(os);
            for(a=a->next;a;a=a->next) a->display_extra(os);
            os << ')';
            return os;
        }

        bool claw:: contains(const species &s) const throw() {
            for(const actor *a=crew.head;a;a=a->next)
            {
                if( &s == & **a ) return true;
            }
            return false;
        }

        bool claw:: is_linked_to(const claw &other) const throw()
        {
            for(const actor *a=other->head;a;a=a->next)
            {
                if(contains(**a)) return true;
            }
            return false;
        }



        void claw:: finalize() throw()
        {
            coerce(nrm2) = 0;
            for(const actor *a=crew.head;a;a=a->next)
            {
                coerce(nrm2) += squared(a->nu);
            }
        }

        bool claw:: excess(const readable<double> &Corg,
                           writable<double>       &Cout,
                           raddops                &xadd) const
        {
            xadd.ldz();
            for(const actor *a=crew.head;a;a=a->next)
            {
                xadd.push( Corg[***a] * a->nu );
            }
            const double xs = xadd.get();
            if(xs<0)
            {
                const double xc = -xs;
                iota::load(Cout,Corg);
                xadd.ldz();
                for(const actor *a=crew.head;a;a=a->next)
                {
                    const double d = (xc * a->nu) / nrm2;
                    Cout[***a] += d;
                    xadd.push(d);
                }
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}

#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace chemical
    {
        void claw:: viz(ios::ostream &fp) const
        {
            for(const actor *a=crew.head;a&&a->next;a=a->next)
            {
                const species &A = **a;
                const species &B = **(a->next);
                const size_t   i = **this;
                const size_t   c = 1 + (i%7);
                A.logo(fp);
                fp << " -> ";
                B.logo(fp);
                fp("[arrowhead=odot,arrowtail=odot,dir=both,color=\"/dark27/%u\"]",unsigned(c));
                A.end(fp);
            }
        }

    }
}
