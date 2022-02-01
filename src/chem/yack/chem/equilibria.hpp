
//! \file

#ifndef YACK_CHEM_EQS_INCLUDED
#define YACK_CHEM_EQS_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! base class for equilibria
        //
        //______________________________________________________________________
        typedef suffix_set<string,const equilibrium::pointer> eqdb;

        //______________________________________________________________________
        //
        //
        //! alias to loop over equilibria
        //
        //______________________________________________________________________
        typedef eqdb::knot_type  enode;

        //______________________________________________________________________
        //
        //
        //! set of equilibria
        //
        //______________________________________________________________________
        class equilibria : public collection
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibria() throw();
            explicit equilibria() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            equilibrium & use(equilibrium *);    //!< register into database
            const enode   *head() const throw(); //!< first equilibrium
            virtual size_t size() const throw(); //!< number of equilibria

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! helper to declare a constant equilibrium
            template <typename NAME> inline
            equilibrium & operator()(const NAME &name, const double K)
            {
                return use( new const_equilibrium(name,K) );
            }

            //! display
            friend std::ostream & operator<<(std::ostream &, const equilibria &);

            //! gather existing equilibria in database
            static void gather(sequence<string> &existing);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
            eqdb db;

        public:
            const size_t width; //!< max name length
        };

    }

}

#endif
