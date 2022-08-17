
#include "yack/chem/components.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    namespace chemical
    {

        const char * components:: state_text(const state s) throw()
        {
            switch (s) {
                case are_blocked: return "[BLOCKED]";
                case are_running: return "[running]";
            }
            return yack_unknown;
        }


        const char components:: clid[] = "chemical::components";
        
        components:: ~components() throw()
        {
        }
        
        
        components:: components() throw() :
        reac(),
        prod(),
        d_nu(0),
        cdb(),
        xlm()
        {
            
        }
        
        std::ostream & operator<<(std::ostream &os, const components &comp)
        {
            os << comp.reac << " <=> " << comp.prod;
            return os;
        }
        
        const cnode * components:: head() const throw()
        {
            return (*cdb.tree).head;
        }
        
        size_t components:: size() const throw()
        {
            return (*cdb.tree).size;
        }

        
        const xlimits & components:: genuine_limits(const readable<double> &C, const size_t w) const throw()
        {
            return * new ( *xlm ) xlimits(reac.genuine_limit(C),prod.genuine_limit(C),w);
        }
        
        void components:: operator()(const species &sp,
                                     const int      nu)
        {

            // create new component
            const component::pointer cptr = new component(sp,nu);
            if(!cdb.insert(cptr)) throw imported::exception(clid,"muliple '%s'",sp.name());

            // update actors
            try {
                switch( __sign::of(nu) )
                {
                    case __zero__:
                        throw imported::exception(clid,"invalid 0 * '%s'",sp.name());

                    case positive:
                        coerce(prod)(sp,nu);
                        break;

                    case negative:
                        coerce(reac)(sp,-nu);
                        break;
                }
            }
            catch(...)
            {
                (void) cdb.remove(sp.name);
                throw;
            }
            
            // update
            ++(coerce(sp.rank));
            coerce(d_nu) = int(prod.molecularity) - int(reac.molecularity);
        }
        
        double components:: mass_action(const double            K,
                                        const readable<double> &C,
                                        rmulops                &ops) const
        {
            // reactant side
            ops = K;
            const double rma = reac.mass_action(C,ops);

            // product side
            ops.set1();
            const double pma = prod.mass_action(C,ops);

            // difference
            return rma - pma;
        }

        void components:: drvs_action(writable<double>       &psi,
                                      const double            K,
                                      const readable<double> &C,
                                      rmulops                &xmul) const
        {
            psi.ld(0);
            reac.drvs_action(psi,    K, C, xmul);
            prod.drvs_action(psi, -1.0, C, xmul);
        }
        
        double components:: quotient(const double            K,
                                     const readable<double> &C,
                                     rmulops                &ops) const
        {
            // reactant side
            ops = K;
            const double den = reac.mass_action(C,ops);

            // product side
            ops.set1();
            const double num = prod.mass_action(C,ops);

            if(fabs(den)<=0)
            {
                if(fabs(num)<=0)
                {
                    return  1;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return (num/den);
            }

        }



        double components:: mass_action(const double            K,
                                        const readable<double> &C,
                                        const double            xi,
                                        rmulops                &ops) const
        {
            // reactant side
            ops = K;
            const double rma = reac.mass_action(C,-xi,ops);

            // product side
            ops.set1();
            const double pma = prod.mass_action(C,xi,ops);

            // difference
            return rma - pma;
        }

       
        
        
        double components:: mass_action(const double            K,
                                        const readable<double> &Cini,
                                        const readable<double> &Cend,
                                        const double            u,
                                        writable<double>       &Ctry,
                                        rmulops                &xmul) const
        {
            if(u<=0)
                return mass_action(K,Cini,xmul);
            else
            {
                if(u>=1.0)
                {
                    return mass_action(K,Cend,xmul);
                }
                else
                {
                    const double v = 1.0 - u;
                    math::iota::load(Ctry,Cini);
                    for(const cnode *node=head();node;node=node->next)
                    {
                        const size_t j = *****node;
                        Ctry[j] = v * Cini[j] + u * Cend[j];
                    }
                    return mass_action(K,Ctry,xmul);
                }
            }
        }

       
        

        double components:: estimate_extent(const readable<double> &Cini,
                                            const readable<double> &Cend,
                                            raddops                &add) const
        {
            add.free();
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                const size_t     j = **c;
                add << (Cend[j]-Cini[j])/c.nu;
            }
            return add.get()/size();
        }


        extent::grade components:: qualify_extent(const double            K,
                                                  const readable<double> &Cend,
                                                  const readable<double> &Cini,
                                                  rmulops                &xmul) const
        {
            const double MA_ini = mass_action(K,Cini,xmul);
            const double MA_end = mass_action(K,Cend,xmul);
            return extent::is_significant;
        }

        void components:: move(writable<double> &C, const double xi) const throw()
        {
            prod.move(C, xi);
            reac.move(C,-xi);
        }


        bool components:: attached_to(const components &other) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t     j = *****node;
                for(const cnode *scan=other.head();scan;scan=scan->next)
                {
                    if(*****scan==j) return true;
                }
            }
            return false;
        }

        bool components:: detached_of(const components &other) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t     j = *****node;
                for(const cnode *scan=other.head();scan;scan=scan->next)
                {
                    if(*****scan==j) return false;
                }
            }
            return true;
        }



    }


}

#include "yack/arith/gcd.h"

namespace yack
{
    namespace chemical
    {

        bool components:: neutral() const throw()
        {
            return reac.algebraic_Z == prod.algebraic_Z;
        }

        bool components:: minimal() const throw()
        {
            if(size()<=1)
            {
                return true;
            }
            else
            {
                assert(size()>=2);
                const cnode   *node = head();              assert(node);
                uint64_t       gcd  = abs( (**node)->nu ); assert(gcd>0);
                for(node=node->next;node;node=node->next)
                {
                    gcd = yack_gcd64(gcd, abs( (**node)->nu ) );
                }
                return 1 == gcd;
            }
        }

        components::state components:: state_at(const readable<double> &C) const throw()
        {
            return (reac.are_blocked_by(C) && prod.are_blocked_by(C)) ? are_blocked : are_running;
        }


    }
    
}
