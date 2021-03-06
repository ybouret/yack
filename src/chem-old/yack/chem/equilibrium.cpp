#include "yack/chem/equilibrium.hpp"
#include "yack/exception.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace chemical
    {


        const char equilibrium:: clid[] = "chemical::equilibrium";
        
        equilibrium:: ~equilibrium() throw()
        {
        }

        
        size_t equilibrium:: size() const throw()
        {
            return comp.size();
        }

        const cnode * equilibrium:: head() const throw()
        {
            return (*comp.tree).head;
        }

        const char * equilibrium:: topo_text() const throw()
        {
            return topology_text(topo);
        }

        bool equilibrium:: uses(const species &sp) const throw()
        {
            return comp.search(sp.name);
        }

        size_t equilibrium:: span() const throw()
        {
            return max_of( reac.span(), prod.span() );
        }

        void equilibrium:: add(const species &sp, const unit_t nu)
        {
            // initialize new component
            const string     &spid = sp.name;
            const component  &c    = coerce(comp).create(sp,nu);
            actors           *a    = 0;

            unit_t delta_p = 0;
            unit_t delta_r = 0;
            // check
            try
            {
                switch( __sign::of(nu) )
                {
                    case __zero__: throw exception("<%s> invalid nu=0 for '%s'", name(), spid() );
                    case negative: a = & coerce(reac); delta_r = -nu; break;
                    case positive: a = & coerce(prod); delta_p =  nu; break;
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
            coerce(nu_p) += delta_p;
            coerce(nu_r) += delta_r;
            coerce(d_nu)  = nu_p - nu_r;
            if(d_nu!=0)
            {
                coerce(sexp) = 1.0 / d_nu;
            }
            else
            {
                coerce(sexp) = 0;
            }

            update_genus();
            build_family();

        }

        void equilibrium:: update_genus() throw()
        {
            if(reac.size)
            {
                if(prod.size)
                {
                    coerce(type) = has_both_ways;
                }
                else
                {
                    coerce(type) = consumes_only;
                }
            }
            else
            {
                if(prod.size)
                {
                    coerce(type) = produces_only;
                }
                else
                {
                    // should never get here
                    coerce(type) = is_unfinished;
                }
            }
        }

#define YACK_EQF(ID) case topology_##ID: return "topology_" #ID

        const char * equilibrium:: topology_text(const topology f) throw()
        {
            switch (f)
            {
                    YACK_EQF(any);
                    YACK_EQF(0_1);
                    YACK_EQF(1_0);
                    YACK_EQF(0_2);
                    YACK_EQF(1_1);
                    YACK_EQF(0_11);
                    YACK_EQF(1_11);
                    YACK_EQF(11_11);
            }
            return yack_unknown;
        }

#define YACK_EQ_FAMILY(R,P) (unsigned(R) << 8 | unsigned(P))

        void equilibrium:: build_family() throw()
        {
            const actors::family r = reac.get_family();
            const actors::family p = prod.get_family();

            const unsigned rp = YACK_EQ_FAMILY(r,p);

            switch(rp)
            {
                case YACK_EQ_FAMILY(actors::empty,actors::kind_1):
                    coerce(topo) = topology_0_1;
                    break;

                case YACK_EQ_FAMILY(actors::kind_1,actors::empty):
                    coerce(topo) = topology_1_0;
                    break;

                case YACK_EQ_FAMILY(actors::empty,actors::kind_2):
                    coerce(topo) = topology_0_2;
                    break;

                case YACK_EQ_FAMILY(actors::kind_1,actors::kind_1):
                    coerce(topo) = topology_1_1;
                    break;

                case YACK_EQ_FAMILY(actors::empty,actors::kind_11):
                    coerce(topo) = topology_0_11;
                    break;

                case YACK_EQ_FAMILY(actors::kind_1,actors::kind_11):
                    coerce(topo) = topology_1_11;
                    break;

                case YACK_EQ_FAMILY(actors::kind_11,actors::kind_11):
                    coerce(topo) = topology_11_11;
                    break;


                default:
                    coerce(topo) = topology_any;
            }

            //std::cerr << "family=" << family_text(kind) << std::endl;


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
                    os << ' ' << '+' << ' ';
                    if(a->nu>1) os << (a->nu);
                    os << (**a).name;
                }
            }
            else
                os << '.';
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
            os << " nu_r: " << nu_r << ", nu_p:" << nu_p << ", d_nu:" << d_nu;
            os << " (" << topo_text() << ")";
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
            assert( span() <= C.size() );
            return reac.mass_action(K0,C) - prod.mass_action(1,C);
        }


        double equilibrium:: drvs_action(writable<double>       &psi,
                                         const double            K0,
                                         const readable<double> &C) const throw()
        {
            assert( span() <= psi.size() );
            assert( span() <= C.size()   );
            psi.ld(0);
            return reac.drvs_action(psi,K0,C) + prod.drvs_action(psi,-1,C);
        }



        const limits & equilibrium:: find_private_limits(const readable<double> &C) const throw()
        {
            return *new ( YACK_STATIC_ZSET(wksp) ) limits( reac.find_private_limiting(C), prod.find_private_limiting(C) );
        }

        const limits & equilibrium:: find_primary_limits(const readable<double> &C) const throw()
        {
            return *new ( YACK_STATIC_ZSET(wksp) ) limits( reac.find_primary_limiting(C), prod.find_primary_limiting(C) );
        }


        bool equilibrium:: is_neutral() const throw()
        {
            unit_t drZ = 0;
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                const species   &s = *c;
                drZ += s.z * c.nu;
            }
            return 0 == drZ;
        }


        writable<double> &equilibrium:: move(writable<double> &Ctmp, const readable<double> &C, const double xi) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const component &cm = ***node;
                const species   &sp = *cm;
                const size_t     j  = *sp;
                const unit_t     nu = cm.nu;
                const double     Cj = C[j];    assert(C[j]>=0);
                Ctmp[j] = max_of<double>(0,Cj + nu * xi);
            }
            return Ctmp;
        }


        double equilibrium:: maximum(const readable<double> &C) const throw()
        {
            return max_of(0.0,max_of( reac.maximum(C), prod.maximum(C) ));
        }

        bool equilibrium:: connexion(const equilibrium &lhs, const equilibrium &rhs) throw()
        {
            assert( &lhs != &rhs );
            for(const cnode *l=lhs.head();l;l=l->next)
            {
                const species &ls = ****l;
                for(const cnode *r=rhs.head();r;r=r->next)
                {
                    const species &rs = ****r;
                    if(&rs==&ls) return true;
                }
                
            }
            return false;
        }

    }

}

#include "yack/arith/gcd.h"

namespace yack
{

    namespace chemical
    {
        void equilibrium:: validate()  const
        {
            const char *id = name();
            if(!size())       throw exception("<%s> is empty", id);
            if(!is_neutral()) throw exception("<%s> has unbalanced charges",id);

            if(size()>1)
            {
                vector<uint64_t> nu( size(), as_capacity );
                for(const actor *a=reac.head;a;a=a->next) nu << a->nu;
                for(const actor *a=prod.head;a;a=a->next) nu << a->nu;
                assert(size()==nu.size());
                uint64_t g = yack_gcd64(nu[1],nu[2]);
                for(size_t i=size();i>2;--i)
                {
                    g = yack_gcd64(g,nu[i]);
                }
                if(1!=g) throw exception("<%s> is not minimal (gcd=%lu)",id, static_cast<long unsigned>(g));
            }


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

        void const_equilibrium:: assign(const double new_value) throw()
        {
            assert(new_value>0);
            coerce(value) = new_value;
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

