//! \file

#ifndef YACK_SYNC_CONTEXT_INCLUDED
#define YACK_SYNC_CONTEXT_INCLUDED 1

#include "yack/concurrent/split1d.hpp"

#include <iosfwd>

namespace yack
{
    class groove;

    namespace concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //! context to locate a thread
        //
        //______________________________________________________________________
        class context
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            context()  noexcept;               //!< sequential
            ~context() noexcept;               //!< cleanup
            context(const size_t sz,
                    const size_t rk) noexcept; //!< parallel

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display C++ style
            friend std::ostream & operator<<(std::ostream &,const context&);

            //! crop part from a larger task
            template <typename U> inline
            void crop(U &length, U &offset) const noexcept
            {
                split1D::with(size,rank,length,offset);
            }

            //! format sz.id
            static
            void format(char buf[], const size_t len, const size_t sz, const size_t id)   noexcept;
          
            //! format size.indx
            void format(char buf[], const size_t len) const noexcept;

            //! on-the-fly access to (created) groove
            groove & operator*() const;

            //! on-the fly access to groove
            groove * operator->() const;

            //! check if groove exists
            bool   owns_local_memory() const noexcept;

            //! get local memory
            size_t local_memory_size() const noexcept;

            //! free local memory if exists
            void free_local_memory() noexcept;

            //! remove local memory if exists
            void drop_local_memory() noexcept;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   size; //!< size>0
            const size_t   rank; //!< rank<size, MPI style
            const size_t   indx; //!< indx=rank+1, C++ style



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(context);
            mutable groove *data; //!< local memory

        };
        
    }

}

#endif
