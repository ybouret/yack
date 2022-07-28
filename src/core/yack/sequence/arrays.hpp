//! \file

#ifndef YACK_ARRAYS_INCLUDED
#define YACK_ARRAYS_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/operative.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! memory and metrics for arrays
    //
    //__________________________________________________________________________
    class arrays : public dynamic, public releasable
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

        size_t capacity; //!< capacity per item
        size_t gathered; //!< capacity*count
        size_t acquired; //!< gathered*block_size
        void  *position; //!< first object

        //! query memory for num_blocks
        void *query(const size_t num_blocks,
                    const size_t block_size,
                    size_t &     gathered_,
                    size_t &     acquired_) const;

        //! allocator release part
        static void deallocate(void * &, size_t &) throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays);
        void release_blocks() throw();
        void release_arrays() throw();

    public:
        virtual size_t granted()     const throw(); //!< bytes for object
        size_t         mutual_size() const throw(); //!< mutual size per array
        size_t         fixed_bytes() const throw(); //!< allocated for arrays
    };


    //__________________________________________________________________________
    //
    //
    //! multiple arrays of same type, same sizes = mutual_size()
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
        typedef thin_array<T>                     array_type;     //!< alias
        typedef writable<array_type>              base_type;      //!< alias
        typedef typename array_type::mutable_type object_type;    //!< alias;
        typedef memory::operative_of<T>           operative_type; //!< alias

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
        mem(position,gathered),
        use(0)
        {
            link();
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

        //______________________________________________________________________
        //
        //! get next array for setup
        //______________________________________________________________________
        array_type & next() throw() { assert(use<size()); return arr[++use]; }

        //______________________________________________________________________
        //
        //! make new mutual size
        //______________________________________________________________________
        inline void make(const size_t n)
        {
            if(n!=capacity)
            {
                rebuild(n);
                assert(n==mutual_size());
            }
        }

        //______________________________________________________________________
        //
        //! ensure mutual size
        //______________________________________________________________________
        inline void ensure(const size_t n)
        {
            if(n>capacity)
            {
                rebuild(n);
                assert(n==mutual_size());
            }
        }

        //______________________________________________________________________
        //
        //! release memory for objects
        //______________________________________________________________________
        inline virtual void release() throw()
        {
            { operative_type nil(NULL,0); mem.swap_with(nil); }
            deallocate(position,acquired);
            capacity = 0;
            gathered = 0;
            link();
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(arrays_of);
        array_type     *arr;
        operative_type  mem;
        size_t          use;

        inline void link() throw()
        {
            object_type *obj = static_cast<object_type*>(position);
            const size_t stp = capacity;
            const size_t num = count;
            for(size_t i=1;i<=num;++i,obj+=stp)
            {
                new (&arr[i]) array_type(obj,capacity);
            }
        }
        
        inline void rebuild(const size_t num_blocks)
        {
            //__________________________________________________________________
            //
            // acquire new resources
            //__________________________________________________________________
            {
                //______________________________________________________________
                //
                // linear part
                //______________________________________________________________
                size_t gathered_ = 0;
                size_t acquired_ = 0;
                void  *position_ = query(num_blocks,sizeof(T),gathered_,acquired_);
                try
                {
                    //__________________________________________________________
                    //
                    // use local operative and update data upon success
                    //__________________________________________________________
                    {
                        operative_type mem_(position_,gathered_);
                        cswap(acquired,acquired_);
                        cswap(position,position_);
                        gathered = gathered_;
                        capacity = num_blocks;
                        mem_.swap_with(mem);
                    }

                    //__________________________________________________________
                    //
                    // clear old memory
                    //__________________________________________________________
                    deallocate(position_,acquired_);
                }
                catch(...)
                {
                    // here, operative setup was a failure
                    deallocate(position_,acquired_);
                    throw;
                }
            }

            //__________________________________________________________________
            //
            // relink
            //__________________________________________________________________
            link();
        }

    };


    


}

#endif

