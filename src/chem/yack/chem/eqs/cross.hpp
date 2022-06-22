
//! \file

#ifndef YACK_CHEM_CROSS_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_CROSS_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! (x,y) stoichiometry
        //
        //______________________________________________________________________
        typedef v2d<int> stoichiometry;

        //______________________________________________________________________
        //
        //
        //! cross equilibrium
        //
        //______________________________________________________________________
        class cross_equilibrium : public equilibrium
        {
        public:
            //! create a name from names and coefficients
            static string make_name(const string &xname, const string &yname, const stoichiometry &st);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename ID> inline
            cross_equilibrium(const ID            &id_,
                              const stoichiometry  st_,
                              const double        &Kx_,
                              const double        &Ky_,
                              const size_t         ic_) :
            equilibrium(id_,ic_),
            st(st_),
            Kx(Kx_),
            Ky(Ky_)
            {

            }

            //! cleanup
            virtual ~cross_equilibrium() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________

            const stoichiometry st; //!< combination
            const double       &Kx; //!< ref to x equilibrium
            const double       &Ky; //!< ref to y equilibrium

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cross_equilibrium);
            virtual double getK(const double) const;

        };
    }

}

#endif
