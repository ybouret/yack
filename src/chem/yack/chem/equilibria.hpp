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
            const enode *tail() const throw(); //!< last  equilibrium

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

            //! display helper
            template <typename PREFIX, typename ARRAY> inline
            void operator()(std::ostream &os, const PREFIX &prefix, ARRAY &arr) const
            {
                os << '{' << std::endl;
                for(const enode *node=head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    pad(os << ' ' << prefix << '<' << eq.name << '>',eq) << " = " << arr[ei] << std::endl;
                }
                os << '}' << std::endl;
            }

            //__________________________________________________________________
            //
            // access
            //__________________________________________________________________
            const equilibrium & operator[](const string &) const; //!< access by name
            const equilibrium & operator[](const char   *) const; //!< access by name
            void remove_last(); //!< removing last inserted equilibrium and update


        private:
            YACK_DISABLE_ASSIGN(equilibria);
            eqs_db edb;


         };

    }
}

#endif
