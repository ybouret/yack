
#include "yack/chem/reactor/claws.hpp"

namespace yack
{
    namespace chemical
    {

        claw::  claw(const size_t i) throw() : object(), indexed(i), next(0), prev(0), crew() {}
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

        bool claw:: attached_to(const claw &other) const throw()
        {
            for(const actor *a=other->head;a;a=a->next)
            {
                if(contains(**a)) return true;
            }
            return false;
        }

        size_t claw:: span() const throw()
        {
            size_t res = 0;
            for(const actor *a=crew.head;a;a=a->next)
            {
                res = max_of<size_t>(res, ***a );
            }
            return res;
        }

    }
}

namespace yack
{
    namespace chemical
    {
        claws::  claws() throw() : object(), claws_() {}
        claws:: ~claws() throw() {}


        size_t claws:: span() const throw()
        {
            size_t res = 0;
            for(const claw *cl = head; cl; cl=cl->next)
            {
                res = max_of(res,cl->span());
            }
            return res;
        }

    }

}

