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

        const string  & equilibrium:: key()  const throw()
        {
            return name;
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
            // initialize new component
            const string     &spid = sp.name;
            const component  &c    = coerce(comp).create(sp,nu);
            actors           *a    = 0;

            // check
            try
            {
                switch( __sign::of(nu) )
                {
                    case __zero__: throw exception("<%s> invalid nu=0 for '%s'", name(), spid() );
                    case negative: a = & coerce(reac); break;
                    case positive: a = & coerce(prod); break;
                }
                assert(NULL!=a);
                a->push_back( new actor(c) );
            }
            catch(...)
            {
                (void) coerce(comp).remove(spid);
                throw;
            }

            // update species
            coerce(sp.rank)++;

        }

        static inline void displayA(std::ostream &os, const actors &A)
        {
            if(A.size)
            {
                const actor *a = A.head;
                if(a->nu>1) os << (a->nu);
                os << (**a).name;
                for(a=a->next;a;a=a->next)
                {
                    os << '+';
                    if(a->nu>1) os << (a->nu);
                    os << (**a).name;
                }
            }
        }

        void equilibrium:: display(std::ostream &os, const size_t w, const double t) const
        {
            os << '<' << name << '>';
            for(size_t i=name.size();i<=w;++i) os << ' ';
            os << " : ";
            displayA(os,reac);
            os << " <=> ";
            displayA(os,prod);
            os << " | K(" << t << ")=" << getK(t);
        }

        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            eq.display(os,0,0);
            return os;
        }

        double equilibrium:: K(double t) const
        {
            const double ans = getK(t);
            if(ans<=0)
            {
                throw exception("<%s> K(%g)=%g", name(), t, ans);
            }
            return ans;
        }


        double equilibrium:: mass_action(const double            K0,
                                         const readable<double> &C) const throw()
        {
            return reac.mass_action(K0,C) - prod.mass_action(1,C);
        }


        void equilibrium:: drvs_action(writable<double>       &phi,
                                       const double            K0,
                                       const readable<double> &C) const throw()
        {
            reac.drvs_action(phi,K0,C);
            prod.drvs_action(phi,-1,C);
        }



        const limits & equilibrium:: find_limits(const readable<double> &C) const throw()
        {
            return *new ( YACK_STATIC_ZSET(wksp) ) limits( reac.find_limiting(C), prod.find_limiting(C) );
        }

        bool equilibrium:: is_neutral() const throw()
        {
            unit_t drZ = 0;
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                drZ += c.sp.z * c.nu;
            }
            return 0 == drZ;
        }


    }

}

namespace yack
{

    namespace chemical
    {

        const_equilibrium:: ~const_equilibrium() throw()
        {
        }

        double const_equilibrium:: getK(double) const
        {
            return value;
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

