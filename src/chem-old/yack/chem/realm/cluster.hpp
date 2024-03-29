//! \file

#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! cluster of detached equilibria for parallel solving
        //
        //______________________________________________________________________
        class cluster : public eq_list
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cluster(const equilibrium &first);             //!< setup with first equilibria
            virtual ~cluster() noexcept;                            //!< cleanup
            explicit cluster(const cluster &, const equilibrium &); //!< duplicate and add

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! accepts if extra index is increasing and extra is detached
            bool       accepts(const matrix<bool> &detached,
                               const equilibrium  &extra) const noexcept;

            //! check for debug
            bool       check() const noexcept;

            //! compare: by size then lexicographic
            static int compare(const cluster *, const cluster *) noexcept;

            
            //! gather list of species
            void       compile();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const sp_list spec; //!< all species
            cluster      *next; //!< for partition
            cluster      *prev; //!< for partition
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };

        //______________________________________________________________________
        //
        //
        //! a partition is a list of clusters
        //
        //______________________________________________________________________
        typedef cxx_list_of<cluster> partition;
        

    }
}


#endif


