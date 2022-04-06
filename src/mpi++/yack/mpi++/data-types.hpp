
//! \file

#ifndef YACK_MPICXX_DATA_TYPES_INCLUDED
#define YACK_MPICXX_DATA_TYPES_INCLUDED 1


#include "yack/system/rtti.hpp"
#include "yack/data/suffix/tree.hpp"

#define OMPI_SKIP_MPICXX 1 //!< helper for OpenMPI
#include <mpi.h>

namespace yack
{

    namespace __mpi
    {
        //______________________________________________________________________
        //
        //
        //! base type for database
        //
        //______________________________________________________________________
        typedef kernel::suffix_tree<MPI_Datatype,uint8_t> data_type_tree;

        //______________________________________________________________________
        //
        //
        //! database of built-in data types for integral types
        //
        //______________________________________________________________________
        class data_types : public data_type_tree
        {
        public:
            explicit data_types();          //!< create and populate database
            virtual ~data_types() throw();  //!< cleanup


            MPI_Datatype        operator()(const rtti &tid) const; //!< access
            const MPI_Datatype *query(const rtti &) const throw(); //!< query

            //! helper
            template <typename T> inline
            bool has() const
            {
                return NULL != query( rtti::use<T>() );
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_types);
            void make(const rtti        &tid,
                      const MPI_Datatype mdt);

            template <typename T> inline
            void in(const MPI_Datatype mdt)
            {
                make( rtti::use<T>(), mdt );
            }
        };

    }

}

#endif
