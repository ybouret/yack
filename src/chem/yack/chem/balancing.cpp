
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/cxx-series.hpp"
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        balancing:: ~balancing() throw()
        {
        }

        balancing:: balancing(const nexus &usr, const xmlog &out) :
        authority<const nexus>(usr),
        xml(out)
        {}

        bool balancing:: operator()(writable<double> &C0)
        {
            bool result = true;
            for(const cluster *cls= (**this).related.head;cls;cls=cls->next)
            {
                if(!balance(C0,*cls)) {
                    result = false;
                }
            }
            return result;
        }

        class xinfo
        {
        public:
            double         x;
            const species &s;

            xinfo(const double _x, const species &_s) throw() : x(_x), s(_s) {}
            ~xinfo() throw() {}
            xinfo(const xinfo &_) throw() : x(_.x), s(_.s)  {}

            inline friend std::ostream & operator<<(std::ostream &os, const xinfo &self)
            {
                os << "<" << self.s.name << ">@" << self.x;
                return os;
            }
        private:
            YACK_DISABLE_ASSIGN(xinfo);
        };

        class xinfos : public cxx_series<xinfo>
        {
        public:
            explicit xinfos(const size_t m) : cxx_series<xinfo>(m) {}
            virtual ~xinfos() throw() {}

            void add(const double x, const species &s) {
                const xinfo _(x,s);
                push_back(_);
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xinfos);
        };


        static inline
        void fill(xinfos                          &neg,
                  xinfos                          &pos,
                  const actor                     *a,
                  const readable<double>          &C,
                  const readable<const criterion> &crit)
        {
            neg.free();
            pos.free();
            for(;a;a=a->next)
            {
                const species &s = **a;
                const size_t   j = *s; if(conserved!=crit[j]) continue;
                const double   c = C[j];
                if(c<0)
                {
                    neg.add( (-c)/a->nu,s );
                }
                else
                {
                    pos.add( c/a->nu,s );
                }
            }
        }

        bool compress_limiting(double                     &xi,
                               sp_repo                    &zs,
                               const readable<xinfo>      &xp)
        {
            xi = 0;
            zs.release();
            const size_t np = xp.size();
            if(np)
            {
                {
                    const xinfo &head = xp[1];
                    xi  =   head.x;
                    zs << & head.s;
                }
                for(size_t i=np;i>1;--i)
                {
                    const xinfo &here = xp[i];
                    switch( __sign::of(here.x,xi) )
                    {
                        case negative: assert(here.x<xi);
                            zs.release();
                            xi = here.x;
                            zs << & here.s;
                            continue;

                        case positive: assert(here.x>xi);
                            continue;

                        case __zero__:
                            zs << & here.s;
                            continue;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }


        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            YACK_XMLSUB(xml, "balancing:cluster");
            static const unsigned balanced         = 0x00;
            static const unsigned unbalanced_reac  = 0x01;
            static const unsigned unbalanced_prod  = 0x02;
            static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;

            const size_t                     M     = (**this).M ;
            const readable<const criterion> &crit = (**this).crit;
            xinfos negative_reac(M);
            xinfos positive_reac(M);
            xinfos negative_prod(M);
            xinfos limiting_prod(M);

            sp_fund fund = new sp_pool();
            sp_repo zs(fund);


            const equilibria &eqs = (**this).lattice;

            YACK_XMLOG(xml," |cluster| =" << cc.size);
            YACK_XMLOG(xml," |roaming| =" << cc.roaming->size);
            YACK_XMLOG(xml," |bounded| =" << cc.bounded->size);

            for(const anode *an = (**this).working.head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;

        TRY_BALANCE:
            std::cerr << "Try Balancing Bounded" << std::endl;
            for(const eq_node *en=cc.bounded->head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                unsigned           flag = balanced;
                eqs.pad(std::cerr << "-> " << eq.name,eq) << " : ";

                fill(negative_reac,positive_reac,eq.reac->head,C0,crit);
                fill(negative_prod,limiting_prod,eq.prod->head,C0,crit);
                if(negative_reac.size()) flag |= unbalanced_reac;
                if(negative_prod.size()) flag |= unbalanced_prod;


                double xi = 0;
                switch(flag)
                {
                    case unbalanced_both:
                        std::cerr << "[unbalanced both]" << std::endl;
                        continue;

                    case unbalanced_reac:
                        std::cerr << "[unbalanced reac] " << negative_reac << " | limiting: " << limiting_prod;
                        if(compress_limiting(xi,zs,limiting_prod) )
                        {
                            std::cerr << " / " << zs << " @" << xi;
                        }
                        break;

                    case unbalanced_prod:
                        std::cerr << "[unbalanced prod] " << negative_prod << " | limiting: " << positive_reac;
                        if(compress_limiting(xi,zs,positive_reac) )
                        {
                            std::cerr << " / " << zs << " @" << xi;
                        }
                        break;

                    default:
                        assert(0==flag);
                        std::cerr << "[balanced]" << std::endl;
                        continue;
                }



                std::cerr << std::endl;
            }


            return false;
        }

    }
}

