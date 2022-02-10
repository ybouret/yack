
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
            equilibrium &       use(equilibrium *);                     //!< register into database
            const enode        *head()                   const throw(); //!< first equilibrium
            virtual size_t      size()                   const throw(); //!< number of equilibria
            const equilibrium & operator[](const size_t) const throw(); //!< (slow) access by index

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

            //! display named values
            template <typename T> inline
            void operator()(std::ostream &os, const readable<T> &arr) const
            {
                os << '{' << std::endl;
                for(const enode *node=head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    pad(os << ' ' << '@' << eq.name,eq.name.size());
                    os << " = " << arr[eq.indx] << std::endl;
                }
                os << '}' << std::endl;
            }

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
            eqdb db;

        public:
            const size_t width; //!< max name length
            std::ostream & pad(std::ostream &,const size_t) const; //!< pad to witdh
        };

    }

}

#endif
