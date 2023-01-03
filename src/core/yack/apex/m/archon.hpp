
//! \file

#ifndef YACK_APEX_ARCHON_INCLUDED
#define YACK_APEX_ARCHON_INCLUDED 1


#include "yack/singleton.hpp"
#include "yack/apex/m/archon-longevity.hpp"
#include "yack/apex/types.hpp"
#include <iostream>

namespace yack
{

    namespace apex
    {
        class hoard;

        //______________________________________________________________________
        //
        //
        //! memory cache for apex
        /**
         underlying allocator is memory::dyadic
         */
        //
        //______________________________________________________________________
        class archon : public singleton<archon>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const at_exit::longevity life_time = YACK_APEX_ARCHON_LONGEVITY; //!< longevity
            static const char               call_sign[];                            //!< "apex::archon"

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void *acquire(exp2_t &block_exp2);                            //!< locked acquire
            void  release(void   *block_addr, exp2_t block_exp2) throw(); //!< locked release

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! acquire a field of 1<<items_exp2 items
            template <typename T> static inline
            T *acquire_field(exp2_t &items_exp2, exp2_t &block_exp2)
            {
                static archon      &self  = instance();
                static const size_t shift = ilog2_of<T>::value;
                assert(0==block_exp2);
                block_exp2 = items_exp2 + shift;
                try {
                    T *field   = static_cast<T *>( self.acquire(block_exp2) );
                    items_exp2 = block_exp2 - shift;
                    return field;
                }
                catch(...)
                {
                    items_exp2 = 0;
                    throw;
                }
            }

            //! release a field of 1<<items_exp2 items
            template <typename T> static inline
            void release_field(T * &entry, exp2_t &items_exp2, exp2_t &block_exp2) throw()
            {
                assert(entry);
                assert(items_exp2+ilog2_of<T>::value==block_exp2);

                static archon &self = location();
                self.release(entry,block_exp2);
                entry = NULL; items_exp2=0; block_exp2=0;
            }

            //__________________________________________________________________
            //
            //! smart pointer for array of T
            //__________________________________________________________________
            template <typename T>
            class tableau
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! acquire items_exp2 >= usr_items_exp2
                inline tableau(const size_t usr_items_exp2) :
                size(1),
                items_exp2(usr_items_exp2),
                block_exp2(0),
                block_addr( acquire_field<T>(coerce(items_exp2),coerce(block_exp2)) )
                {
                    coerce(size) <<= items_exp2;
                }

                //! cleanup
                inline ~tableau() throw()
                {
                    release_field(block_addr,coerce(items_exp2),coerce(block_exp2));
                    coerce(size)=0;
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! access [0..size-1]
                inline T       & operator[](const size_t indx) throw()       { assert(indx<size); return block_addr[indx]; }

                //! const access[0..size-1]
                inline const T & operator[](const size_t indx) const throw() { assert(indx<size); return block_addr[indx]; }

                //! internal bytes
                size_t bytes() const throw()
                {
                    return size_t(1) << block_exp2;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t size; //!< in items

            private:
                const exp2_t items_exp2; //!< size = 1 << items_exp2
                const exp2_t block_exp2; //!< items_exp2 + log2(sizeof(T))
                T           *block_addr; //!< memory
                YACK_DISABLE_COPY_AND_ASSIGN(tableau);
            };

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(archon);
            explicit archon() throw();
            virtual ~archon() throw();
            friend class singleton<archon>;
            hoard *cache; //!< internal cache
        };



    }

}

#endif
