//! \file

#ifndef YACK_CHEM_EQUILIBRIA_INCLUDED
#define YACK_CHEM_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/compendium.hpp"


namespace yack
{
    namespace chemical
    {
        class library;

        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef suffix_set<string,const equilibrium::pointer> eq_set; //!< alias
        typedef eq_set::knot_type                             enode;  //!< alias

        //______________________________________________________________________
        //
        //
        //!set of unique equilibrium
        //
        //______________________________________________________________________
        class equilibria : public compendium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria() throw(); //!< setup empty
            virtual ~equilibria() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual size_t size() const throw(); //!< size
            const enode   *head() const throw(); //!< first equilibrium

            //! try to insert a new equilibrium
            equilibrium & use( equilibrium *eq );

            //! try to declare a new constant equilibrium
            template <typename ID> inline
            equilibrium & create(const ID &name, const double K)
            {
                return use( new const_equilibrium(name,db.size()+1,K) );
            }

            friend std::ostream & operator<<(std::ostream &, const equilibria &);

         
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
            eq_set db;
        };

    }

}

#endif

