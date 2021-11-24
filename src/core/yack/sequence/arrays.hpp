//! \file

#ifndef YACK_ARRAYS_INCLUDED
#define YACK_ARRAYS_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{


    class arrays : public collection
    {
    public:
        virtual ~arrays() throw();

        virtual size_t size() const throw();

        explicit arrays(const size_t num_arrays,
                        const size_t block_size,
                        const size_t num_blocks);

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays);
        size_t count; //!< arrays: count => size()
        size_t bytes; //!< arrays: memory byte
        void  *entry; //!< arrays: first one

        size_t capacity; //!< max items per array
        size_t acquired; //!< capacity*count*block_size
        void  *position; //!< first object

    private:
        void release_arrays() throw();
        void release_blocks() throw();

    };

#if 0
    class arrays
    {
    public:
        virtual ~arrays() throw();

        explicit arrays(const size_t num_arrays,
                        const size_t block_size,
                        const size_t num_blocks);

        const size_t count;
        const size_t items;

    protected:
        size_t       arr_bytes;
        void        *arr_entry;
        size_t       obj_bytes;
        void        *obj_entry;


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays);
        void release_objets() throw();
        void release_arrays() throw();
    };

    template <typename T>
    class arrays_of : public arrays
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef thin_array<T> array_type;

        inline virtual ~arrays_of() throw() {}

        inline explicit arrays_of(size_t num_arrays,
                                  size_t num_blocks) :
        arrays(num_arrays,sizeof(T),num_blocks),
        arr(static_cast<array_type *>(arr_entry)-1),
        used(0)
        {
            link();
        }

        inline array_type &operator[](const size_t indx) throw()
        {
            assert(indx>0);
            assert(indx<=count);
            return arr[indx];
        }

        inline const array_type &operator[](const size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=count);
            return arr[indx];
        }

        inline array_type &next() throw()
        {
            assert(used<count);
            return arr[++used];
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays_of);
        array_type *arr;
        size_t      used;

        inline void link() throw()
        {
            std::cerr << "linking..." << std::endl;
            mutable_type *obj = static_cast<mutable_type *>(obj_entry);
            for(size_t i=1;i<=count;++i,obj+=items)
            {
                new ( &arr[i] ) array_type(obj,items);
            }
        }
    };
#endif


}

#endif

