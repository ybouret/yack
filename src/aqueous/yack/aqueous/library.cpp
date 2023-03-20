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

        double library:: conc(randomized::bits &ran) noexcept
        {
            return pow(10.0,pmin + ran.to<double>() * (pmax-pmin));
        }

        void library:: conc(writable<double> &C, randomized::bits &ran) const
        {
            for(const snode *node=(*this)->head;node;node=node->next)
            {
                C[ (***node).indx[0] ] = conc(ran);
            }
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
            os << '{' << std::endl;
            for(const snode *node=self->head;node;node=node->next)
            {
                const species &s = ***node;
                self.pad(std::cerr << '\t' << s,s)
                << " | z=" << std::setw(3) << s.z
                << " | #="  << s.n
                << std::endl;
            }
            os << '}';
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

        void library:: viz(ios::ostream         &fp,
                           const readable<bool> &reg) const
        {
            for(const snode *sn=(*this)->head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                sp.viz(fp,reg[sp.indx[top_level]]);
            }
        }


    }

}

