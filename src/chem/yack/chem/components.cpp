#include "yack/chem/components.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace chemical
    {

        components:: ~components() throw()
        {
        }

        components:: components() throw() : components_()
        {
        }


        const component & components:: create(const species  &sp,
                                 const unit_t    nu)
        {
            const component::pointer p = new component(sp,nu);
            if(!insert(p)) throw exception("chemical::components: multiple '%s'", sp.name());
            return *p;
        }

        void components:: display(std::ostream &os) const
        {
            
            if(size()>0)
            {
                const const_iterator done = end();
                const_iterator       curr = begin();
                (**curr).display(os,true);
                while(++curr!=done)
                {
                    (**curr).display(os,false);
                }
            }
        }

        std::ostream & operator<<(std::ostream &os, const components &c)
        {
            c.display(os);
            return os;
        }

    }

}


