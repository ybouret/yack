//! \file

#ifndef YACK_MPICXX_DATA_TYPES_INCLUDED
#define YACK_MPICXX_DATA_TYPES_INCLUDED 1

#include "yack/mpi++/data/type.hpp"
#include "yack/system/rtti.hpp"
#include "yack/data/suffix/tree.hpp"


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
        typedef kernel::suffix_tree<const data_type,uint8_t> data_type_tree;

        //______________________________________________________________________
        //
        //
        //! database of built-in data types for integral types
        //
        //______________________________________________________________________
        class data_types : public data_type_tree
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit data_types();          //!< create and populate database
            virtual ~data_types() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const data_type & operator()(const rtti &tid) const; //!< access
            const data_type * query(const rtti &) const throw(); //!< query

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_types);
            void make(const rtti        &tid,
                      const MPI_Datatype mdt,
                      const unsigned     bpi);

            template <typename T> inline
            void in(const MPI_Datatype mdt)
            {
                make( rtti::use<T>(), mdt, sizeof(T) );
            }
        };

    }

}

#endif
