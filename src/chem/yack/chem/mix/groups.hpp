
//! \file

#ifndef YACK_CHEM_MIX_GROUPS_INCLUDED
#define YACK_CHEM_MIX_GROUPS_INCLUDED 1

#include "yack/chem/mix/group.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! groups sharing same properties
        //
        //______________________________________________________________________
        class groups : public object, public group::list
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef int2type<0> build_attached_;
            typedef int2type<1> build_clusters_;
            static const build_attached_ build_attached;
            static const build_clusters_ build_clusters;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~groups() throw(); //!< cleanup
            explicit groups() throw(); //!< setup

            //! create groups of ATTACHED equilibria, and set info accordingly
            explicit groups(const equilibria &eqs, const build_attached_ &);

            //! create groups of DETACHED equilibria
            /**
             reply to : what are the equilibrium within an attached group
             that can be tested simultaneously ?
             */
            explicit groups(const group  &attached, const matrix<bool> &detached);

            //! assemble groups from partitions of groups
            explicit groups(const readable<groups> &part);

            //! full algorithm
            explicit groups(const equilibria &lattice, const build_clusters_ &);

            //! dynamic
            static groups *create_from(const equilibria &lattice);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void sort(); //!< full sort groups

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &os, const groups &); //!< display

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(groups);
        };

    }


}

#endif

