
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

    }

}


