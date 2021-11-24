//! \file

#ifndef YACK_ARRAYS_INCLUDED
#define YACK_ARRAYS_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! memory and metrics for arrays
    //
    //__________________________________________________________________________
    class arrays
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        virtual ~arrays() throw();

    protected:
        //! setup all memory
        explicit arrays(const size_t num_arrays,
                        const size_t block_size,
                        const size_t num_blocks);
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        size_t count; //!< arrays: count => size()
        size_t bytes; //!< arrays: memory byte
        void  *entry; //!< arrays: first one

        size_t capacity; //!< max items per array
        size_t gathered; //!< capacity*count
        size_t acquired; //!< gathered*block_size
        void  *position; //!< first object

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays);
        void release_arrays() throw();
        void release_blocks() throw();

    };


    //__________________________________________________________________________
    //
    //
    //! multiple arrays of same type, same size
    //
    //__________________________________________________________________________
    template <typename T>
    class arrays_of : public arrays, public writable< thin_array<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef thin_array<T>        array_type; //!< alias
        typedef writable<array_type> base_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~arrays_of() throw() {}

        //! setup memory
        inline explicit arrays_of(const size_t num_arrays,
                                  const size_t num_blocks) :
        arrays(num_arrays,sizeof(T),num_blocks),
        arr( static_cast<array_type*>(entry) - 1 ),
        mem(position,gathered)
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! collection interface
        inline virtual size_t size() const throw() { return count; }

        //! readable interface
        inline virtual const array_type &operator[](const size_t indx) const throw()
        {
            assert(NULL!=arr); assert(indx>=1); assert(indx<=size()); return arr[indx];
        }

        //! writable interface
        inline virtual array_type &operator[](const size_t indx) throw()
        {
            assert(NULL!=arr); assert(indx>=1); assert(indx<=size()); return arr[indx];
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays_of);
        array_type              *arr;
        memory::operative_of<T>  mem;
    };


}

#endif

