#include "yack/aqueous/library.hpp"
#include "yack/system/imported.hpp"

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



        const species_db::tree_type * library:: operator->() const noexcept
        {
            return & sdb.tree;
        }

        const species_db::tree_type & library:: operator*() const noexcept
        {
            return sdb.tree;
        }

        const species & library:: grow(species *sp)
        {
            assert(NULL!=sp);
            const species::ptr p = sp; assert(sdb.size()+1==sp->primary);
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




    }

}

