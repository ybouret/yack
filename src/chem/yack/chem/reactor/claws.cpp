
#include "yack/chem/reactor/claws.hpp"

namespace yack
{
    namespace chemical
    {

        claw::  claw() throw() : object(), next(0), prev(0), crew() {}
        claw:: ~claw() throw() {}


        void claw::add(const species  &sp,
                       const unsigned  cf)
        {
            crew.push_back( new actor(sp,cf) );
        }

        const list_of<actor> & claw:: operator*()  const throw() { return  crew; }
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
    }
}

namespace yack
{
    namespace chemical
    {
        claws::  claws() throw() : object(), claws_() {}
        claws:: ~claws() throw() {}
    }

}

