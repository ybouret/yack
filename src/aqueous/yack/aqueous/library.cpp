#include "yack/aqueous/library.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    namespace aqueous
    {
        const char * const library:: clid = "aqueous::library";
        
        library:: ~library() noexcept
        {
        }

        library:: library() noexcept : gathering(), sdb()
        {

        }



        const species_db::tree_type & library:: operator->() const noexcept
        {
            return sdb.tree;
        }

        

        const species & library:: grow(species *sp)
        {
            assert(NULL!=sp);
            const species::ptr p = sp; assert(sdb.size()+1==sp->indx[0]);
            if(!sdb.insert(p)) throw imported::exception(clid,"multiple [%s]", p->name());
            update(*p);
            return *p;
        }

        const species & library:: operator[](const string &name) const
        {
            const species::ptr *pp = sdb.search(name);
            if(!pp) throw imported::exception(clid,"unknow [%s]",name());
            return **pp;
        }

        const species & library:: operator[](const char   *name) const
        {
            const string _(name); return (*this)[_];
        }

        std::ostream & operator<<(std::ostream &os, const library &self)
        {
            for(const snode *node=self->head;node;node=node->next)
            {
                const species &s = ***node;
                self.pad(std::cerr << s,s)
                << " | z=" << std::setw(3) << s.z
                << " | #="  << s.n
                << std::endl;
            }

            return os;
        }

        const species * library:: query(const string &name) const noexcept
        {
            const species::ptr *pp = sdb.search(name);
            return pp ? & **pp : NULL;
        }

        const species * library:: query(const char  *name) const
        {
            const string _(name);
            return query(_);
        }



    }

}

