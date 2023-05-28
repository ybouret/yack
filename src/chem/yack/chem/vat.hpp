//! \file

#ifndef YACK_CHEM_VAT_INCLUDED
#define YACK_CHEM_VAT_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
namespace yack
{
    namespace Chemical
    {
        typedef cxx_list_of<Cluster> Clusters;

        class Vat : public Clusters
        {
        public:
            virtual ~Vat() noexcept;
            explicit Vat(const Library &lib,
                         Equilibria    &eqs,
                         const xmlog   &xml);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Vat);
            void makeClusters(const xmlog &, Equilibria &);
            void tryReduce() noexcept;
        };
    }

}

#endif

