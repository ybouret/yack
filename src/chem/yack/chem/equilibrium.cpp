#include "yack/chem/equilibrium.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace chemical
    {

        equilibrium:: ~equilibrium() throw()
        {
        }

        const components  & equilibrium:: bulk() const throw()
        {
            return comp;
        }

        size_t equilibrium:: size() const throw()
        {
            return comp.size();
        }

        const cnode * equilibrium:: head() const throw()
        {
            return (*comp.tree).head;
        }


        void equilibrium:: add(const species &sp, const unit_t nu)
        {
            const string     &spid = sp.name;
            const component  &c    = coerce(comp).create(sp,nu);
            actors           *a    = 0;

            switch( __sign::of(nu) )
            {
                case __zero__: throw exception("<%s> invalid nu=0 for '%s'", name(), spid() );
                case negative: a = & coerce(reac); break;
                case positive: a = & coerce(prod); break;
            }

            assert(NULL!=a);
            try
            {
                a->push_back( new actor(c) );
            }
            catch(...)
            {
                (void) coerce(comp).remove(spid);
                throw;
            }

        }


        void equilibrium:: display(std::ostream &os, const size_t w, const double t) const
        {
            os << '<' << name << '>';
            for(size_t i=name.size();i<=w;++i) os << ' ';
            os << " : ";
            os << comp;
        }

        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            eq.display(os,0,0);
            return os;
        }
    }

}


#include "yack/chem/builder.hpp"

namespace yack
{

    namespace chemical
    {

        void equilibrium:: load(const string &expr, library &lib)
        {
            static builder &mgr = builder::instance();
            mgr.compile(*this,expr,lib);
        }


        void equilibrium:: load(const char *expr, library &lib)
        {
            const string _(expr);
            return load(_,lib);
        }



    }

}

