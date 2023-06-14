
#include "yack/chem/eqs/actors.hpp"

namespace yack
{
    namespace Chemical
    {

        Actors:: ~Actors() noexcept
        {

        }

        static const char noname[] = ".";

        Actors:: Actors() : Actors_(), Entity(noname)
        {
            
        }

        Actors:: Actors(const Actors &other) : Actors_(other), Entity(other.name)
        {

        }


        std::ostream & operator<<(std::ostream &os, const Actors &self)
        {
            const Actor *a = self.head;
            if(a)
            {
                a->displayFirst(os);
                for(a=a->next;a;a=a->next)
                {
                    a->displayExtra(os);
                }
            }
            return os;
        }


        void   Actors:: makeName()
        {
            string &id = coerce(name);
            id.clear();
            const Actor *a =  head;
            if(a)
            {
                id += a->displayFirst();
                for(a=a->next;a;a=a->next)
                {
                    id += a->displayExtra();
                }
            }
        }

        void Actors:: massAction(Extended::Mul                  &xmul,
                                 const readable<Extended::Real> &C,
                                 const IndexLevel                level) const
        {
            for(const Actor *ac = head; ac; ac=ac->next)
            {
                ac->massAction(xmul,C,level);
            }
        }


        void Actors:: gradAction(writable<Extended::Real>       &out,
                                 const IndexLevel                outLevel,
                                 const Extended::Real           &fac,
                                 const readable<Extended::Real> &inp,
                                 const IndexLevel                inpLevel,
                                 Extended::Mul                  &xmul) const
        {

            for(const Actor *ac = head; ac; ac=ac->next)
            {
                const Species &sp = **ac;

                // initialize
                xmul.free();
                xmul.insert(fac);

                // derivative w.r.t input
                xmul.insert(ac->xn);
                xmul.insert(inp[sp.indx[inpLevel]],ac->nm);

                // remaining terms
                for(const Actor *b=ac->prev;b;b=b->prev) b->massAction(xmul,inp,inpLevel);
                for(const Actor *b=ac->next;b;b=b->next) b->massAction(xmul,inp,inpLevel);


                // populate output
                out[sp.indx[outLevel]] = xmul.reduce();

            }

        }
    }

}


