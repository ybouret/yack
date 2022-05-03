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
            context()  throw();               //!< sequential
            ~context() throw();               //!< cleanup
            context(const size_t sz,
                    const size_t rk) throw(); //!< parallel

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display C++ style
            friend std::ostream & operator<<(std::ostream &,const context&);

            //! crop part from a larger task
            template <typename U> inline
            void crop(U &length, U &offset) const throw()
            {
                split1D::with(size,rank,length,offset);
            }

            //! format size.indx
            void format(char buf[], const size_t len) const throw();

            //! on-the-fly access to (created) groove
            groove & operator*() const;
            
            //! check if groove exists
            bool has_local_memory() const throw();
            
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
