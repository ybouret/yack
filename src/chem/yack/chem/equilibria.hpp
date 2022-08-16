//! \file

#ifndef YACK_CHEMICAL_EQS_INCLUDED
#define YACK_CHEMICAL_EQS_INCLUDED 1

#include "yack/chem/eqs/constant.hpp"
#include "yack/chem/gathering.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // global definition
        //
        //______________________________________________________________________
        typedef suffix_set<string,const equilibrium::pointer> eqs_db; //!< set of equilibrium
        typedef eqs_db::knot_type                             enode;  //!< alias
        

        //______________________________________________________________________
        //
        //
        //! set of equilibria
        //
        //______________________________________________________________________
        class equilibria : public gathering
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< "equilibria"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria() throw();  //!< setup empty
            virtual ~equilibria() throw();  //!< cleanup
            equilibria(const equilibria &); //!< shared copies

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t       size() const throw(); //!< number of equilibria
            const enode *head() const throw(); //!< first equilibrium

            //! register a new equilibrium with proper index
            equilibrium & use(equilibrium *eq);

            //! add shared copy
            void add(const equilibria &other);


            //! helper to create a new constant equilibrium
            template <typename NAME> inline
            equilibrium & operator()(const NAME  &uid,
                                     const double K)
            {
                return use( new const_equilibrium(uid,size()+1,K) );
            }

            //! display
            friend std::ostream & operator<<(std::ostream &, const equilibria &eqs);
            

        private:
            YACK_DISABLE_ASSIGN(equilibria);
            eqs_db edb;
         };

    }
}

#endif
