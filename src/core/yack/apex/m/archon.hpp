
//! \file

#ifndef YACK_APEX_ARCHON_INCLUDED
#define YACK_APEX_ARCHON_INCLUDED 1


#include "yack/singleton.hpp"
#include "yack/apex/m/archon-longevity.hpp"

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
            static const at_exit::longevity life_time = YACK_APEX_ARCHON_LONGEVITY; //!< longevity
            static const char               call_sign[];                            //!< "apex::archon"

            void *acquire(size_t &block_exp2);                            //!< locked acquire
            void  release(void   *block_addr, size_t block_exp2) throw(); //!< locked release

            //! acquire a field of 1<<items_exp2 items
            template <typename T> static inline
            T *acquire_field(size_t &items_exp2, size_t &block_exp2)
            {
                static archon      &self  = instance();
                static const size_t shift = ilog2_of<T>::value;
                assert(0==block_exp2);
                block_exp2 = items_exp2 + shift;
                try {
                    T *field   = static_cast<void *>( self.acquire(block_exp2) );
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
            void release_field(T * &entry, size_t &items_exp2, size_t &block_exp2) throw()
            {
                static archon &self = location();
                assert(entry);
                assert(items_exp2==block_exp2+ilog2_of<size_t>::value);
                self.release(entry,block_exp2);
                entry = NULL; items_exp2=0; block_exp2=0;
            }



        private:
            explicit archon() throw();
            virtual ~archon() throw();
            friend class singleton<archon>;
            hoard *cache;

        };
    }

}

#endif
