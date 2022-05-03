//! \file

#ifndef YACK_GROOVE_INCLUDED
#define YACK_GROOVE_INCLUDED 1


#include "yack/system/rtti.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! groove of flexible data
    //
    //__________________________________________________________________________
    class groove : public object, public dynamic
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef void (*on_delete)(void *); //!< anonymous destructor proto

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit groove() throw();
        virtual ~groove() throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual size_t granted() const throw(); //!< bytes
        size_t   size()          const throw(); //!< num_blocks
        void     free()                throw(); //!< remove content, keep bytes
        void     release()             throw(); //!< remove content, remove bytes
        void     ensure(const size_t nb, const size_t bs); //!< free/acquire

        //______________________________________________________________________
        //
        //! build n items
        //______________________________________________________________________
        template <typename T> inline
        void build(const size_t n)
        {
            ensure(n,sizeof(T));
            if(n>0)
            {
                try
                {
                    block_size = sizeof(T);
                    kill_block = kill<T>;
                    block_type = & rtti::use<T>();
                    uint8_t *p = static_cast<uint8_t *>(entry);
                    while(num_blocks<n)
                    {
                        new (p) T();
                        ++num_blocks;
                        p += block_size;
                    }
                }
                catch(...)
                {
                    free();
                    throw;
                }
            }
        }

        //______________________________________________________________________
        //
        //! build one item
        //______________________________________________________________________
        template <typename T> inline
        void build()
        {
            build<T>(1);
        }

        //! check type
        template <typename T> inline
        bool is() const
        {
            static const rtti * const local_type = & rtti::use<T>();
            return local_type == block_type;
        }

        //! access in [0:size()-1]
        template <typename T> inline
        const T & get(const size_t i) const throw()
        {
            assert(i<num_blocks);
            const T *p = static_cast<const T *>(entry);
            return p[i];
        }

        //! access in [0:size()-1]
        template <typename T> inline
        T & get(const size_t i) throw()
        {
            assert(i<num_blocks);
            T *p = static_cast<T *>(entry);
            return p[i];
        }


        template <typename T> inline T       &as()       throw() { return get<T>(0); }  //!< first item
        template <typename T> inline const T &as() const throw() { return get<T>(0); }  //!< first item



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(groove);
        void       *entry;
        size_t      bytes;
        size_t      block_size;
        size_t      num_blocks;
        on_delete   kill_block;
        const rtti *block_type;

        template <typename T> static inline
        void kill(void *addr) throw()
        { assert(NULL!=addr); destruct( static_cast<T*>(addr) );}


    };

}

#endif

