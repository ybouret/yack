
//! \file

#ifndef YACK_MPICXX_DATA_RS_INCLUDED
#define YACK_MPICXX_DATA_RS_INCLUDED 1


#include "yack/mpi++/data/io.hpp"
#include "yack/mpi++/data/types.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{


    namespace __mpi
    {
        
        //______________________________________________________________________
        //
        //! Data Recv/Send
        //______________________________________________________________________
        class data_rs
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::ro_buffer           key_type; //!< alias
            typedef ark_ptr<key_type,data_io>   io_ptr;   //!< alias
            typedef suffix_set<key_type,io_ptr> io_set;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit data_rs(const data_types &native); //!< setup with built-in types
            virtual ~data_rs() throw();                 //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void            declare(data_io *ops);   //!< declare a new set of ops
            const data_io & get(const rtti &) const; //!< get by RTTI

            //! generic helper
            template <typename T> inline
            const data_io & get() const
            {
                static const rtti &tid = rtti::use<T>();
                return get(tid);
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_rs);
            io_set db;
            void make_built_in(const data_types &native);
            void make_cxx_from(const data_types &native);
        };

    }

}

#endif
