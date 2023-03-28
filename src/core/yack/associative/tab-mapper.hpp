
//! \file

#ifndef YACK_ASSOCIATIVE_TAB_MAPPER_INCLUDED
#define YACK_ASSOCIATIVE_TAB_MAPPER_INCLUDED 1


#include "yack/associative/key-mapper.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! mapping global to local indices in 2D
    //
    //__________________________________________________________________________
    template <typename T>
    class tab_mapper
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef key_mapper<T,T>             kmap;          //!< 1D mapper
        typedef typename kmap::fwd_iterator fwd_iterator;  //!< alias
        typedef typename kmap::rev_iterator rev_iterator;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup from persistent mapper
        inline explicit tab_mapper(const kmap &r,
                                   const kmap &c) noexcept :
        rows(r), cols(c)
        {
        }

        //! cleanup
        inline virtual ~tab_mapper() noexcept
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! send global source to local target
        template <typename TARGET, typename SOURCE>
        inline void send(TARGET &target, SOURCE &source) const
        {
            fwd_iterator fwd = rows.forward.begin();
            rev_iterator rev = rows.reverse.begin();
            for(size_t i=rows.size;i>0;--i,++fwd,++rev)
            {
                cols.send(target[ *fwd ],source[ *rev ]);
            }
        }

        //! recv global target fron local source
        template <typename TARGET, typename SOURCE>
        inline void recv(TARGET &target, SOURCE &source) const
        {
            fwd_iterator fwd = rows.forward.begin();
            rev_iterator rev = rows.reverse.begin();
            for(size_t i=rows.size;i>0;--i,++fwd,++rev)
            {
                cols.recv(target[ *rev ],source[ *fwd ]);
            }
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const kmap &rows; //!< mapping rows indices
        const kmap &cols; //!< mapping cols indices

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(tab_mapper);
    };

}

#endif
