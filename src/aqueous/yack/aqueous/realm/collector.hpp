
//! \file

#ifndef YACK_AQUEOUS_COLLECTOR_INCLUDED
#define YACK_AQUEOUS_COLLECTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        
        class survey
        {
        public:
            double   extent;      //!< limiting extent
            sp_repo  vanish;      //!< making these species vanish
            double   request;     //!< minimal request
            sp_repo  nullify;     //!< to nullify those species

            survey(const sp_proxy &proxy) noexcept ;
            ~survey() noexcept;

            void reset() noexcept;
            void probe(const actors           &A,
                         const readable<double> &C);

            friend std::ostream & operator<<(std::ostream &os, const survey &self)
            {
                if(self.vanish.size) {
                    os << "vanish: " << self.vanish << " @" << self.extent;
                }
                else
                {
                    os << "no vanish";
                }
                os << " | ";
                if(self.nullify.size)
                {
                    os << "nullify: " << self.nullify << " @" << self.request;
                }
                else
                {
                    os << "no nullify";
                }
                return os;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(survey);
            void on_positive(const double xi, const species &s, bool &first);
        };

        class surveys
        {
        public:
            surveys(const sp_proxy &proxy) noexcept :
            reac(proxy),
            prod(proxy)
            {
            }

            ~surveys() noexcept
            {
            }

            void reset() noexcept
            {
                reac.reset();
                prod.reset();
            }

            void probe(const components &eq, const readable<double> &C)
            {
                reac.probe(eq.reac,C);
                prod.probe(eq.prod,C);
            }

            friend std::ostream & operator<<(std::ostream &os,
                                      const surveys &self)
            {
                os << "reac: " << self.reac << " | prod: " << self.prod;
                return os;
            }

            survey reac;
            survey prod;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(surveys);
        };


        class collector : public sp_proxy
        {
        public:

            //! 
            explicit collector(const size_t n) :
            sp_proxy( new sp_zpool() ),
            surv(n,static_cast<const sp_proxy&>(*this))
            {
            }

            virtual ~collector() noexcept {}

            void reset() noexcept
            {
                for(size_t i=surv.size();i>0;--i) surv[i].reset();
            }

            void probe(const domain &dom, const readable<double> &C)
            {
                for(const eq_node *node=dom.defined.head;node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = eq.indx[sub_level];
                    surv[ei].probe(eq,C);
                    std::cerr << eq << " => " << surv[ei] << std::endl;
                }
            }

            cxx_array<surveys>  surv;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
        };

    }

}

#endif

