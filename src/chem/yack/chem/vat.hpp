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
        typedef cxx_list_of<Cluster> Clusters; //!< alias

        //______________________________________________________________________
        //
        //
        //! Vat of equilibria
        //
        //______________________________________________________________________
        class Vat : public Clusters
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Vat() noexcept;           //!< cleanup

            //! setup
            explicit Vat(const xmlog   &xml,
                         Equilibria    &eqs);
            



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Vat);
            void makeClusters(const xmlog &, Equilibria &);
        };
    }

}

#endif

