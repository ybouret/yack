//! \file

#ifndef YACK_SORT_NETWORK_INCLUDED
#define YACK_SORT_NETWORK_INCLUDED

#include "yack/sort/nwswp.hpp"
#include "yack/memory/buffer/fixed.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/sequence/thin-array.hpp"
#include <cmath>

namespace yack
{

    namespace nwsrt
    {
        //______________________________________________________________________
        //
        //
        //! algorithm interface
        //
        //______________________________________________________________________
        class algorithm
        {
        public:
            const memory::ro_buffer & key()  const noexcept; //!< wrapped code.name
            virtual                  ~algorithm()  noexcept; //!< cleanup
        protected:
            explicit algorithm(const swaps &)      noexcept; //!< setup

        public:  const swaps               &code;           //!< persistent code
        private: const memory::fixed_buffer buff;
            YACK_DISABLE_COPY_AND_ASSIGN(algorithm);
            
        public:
            //__________________________________________________________________
            //
            //! default increasing order
            //__________________________________________________________________
            template <typename ARRAY> inline
            void increasing(ARRAY &arr) const
            {
                assert(arr.size()==code.size);
                const size_t  nswp = code.work;
                const size_t *ltab = code.ltab;
                const size_t *rtab = code.rtab;
                for(size_t k=0;k<nswp;++k)
                {
                    typename ARRAY::type &l = arr[ ltab[k] ];
                    typename ARRAY::type &r = arr[ rtab[k] ];
                    if(r<l) out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                }
            }

            //__________________________________________________________________
            //
            //! default co-sorting in increasing order
            //__________________________________________________________________
            template <typename ARRAY, typename BRRAY>   inline
            void increasing(ARRAY &arr, BRRAY &brr) const
            {
                assert(arr.size()==code.size);
                assert(brr.size()==code.size);
                const size_t  nswp = code.work;
                const size_t *ltab = code.ltab;
                const size_t *rtab = code.rtab;
                for(size_t k=0;k<nswp;++k)
                {
                    const size_t         il = ltab[k];
                    const size_t         ir = rtab[k];
                    typename ARRAY::type &l = arr[il];
                    typename ARRAY::type &r = arr[ir];
                    if(r<l)
                    {
                        out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                        out_of_reach::swap(&brr[il],&brr[ir],sizeof(typename BRRAY::type));
                    }
                }
            }

            //__________________________________________________________________
            //
            //! default decreasing order
            //__________________________________________________________________
            template <typename ARRAY>   inline
            void decreasing(ARRAY &arr) const
            {
                assert(arr.size()==code.size);
                const size_t  nswp = code.work;
                const size_t *ltab = code.ltab;
                const size_t *rtab = code.rtab;
                for(size_t k=0;k<nswp;++k)
                {
                    typename ARRAY::type &l = arr[ ltab[k] ];
                    typename ARRAY::type &r = arr[ rtab[k] ];
                    if(l<r) out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                }
            }

            //__________________________________________________________________
            //
            //! default co-sorting in decreasing order
            //__________________________________________________________________
            template <typename ARRAY, typename BRRAY>   inline
            void decreasing(ARRAY &arr, BRRAY &brr) const
            {
                assert(arr.size()==code.size);
                assert(brr.size()==code.size);
                const size_t  nswp = code.work;
                const size_t *ltab = code.ltab;
                const size_t *rtab = code.rtab;
                for(size_t k=0;k<nswp;++k)
                {
                    const size_t         il = ltab[k];
                    const size_t         ir = rtab[k];
                    typename ARRAY::type &l = arr[il];
                    typename ARRAY::type &r = arr[ir];
                    if(l<r)
                    {
                        out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                        out_of_reach::swap(&brr[il],&brr[ir],sizeof(typename BRRAY::type));
                    }
                }
            }

            //__________________________________________________________________
            //
            //!   decreasing order by absolute value
            //__________________________________________________________________
            template <typename ARRAY>   inline
            void decreasing_abs(ARRAY &arr) const
            {
                assert(arr.size()==code.size);
                const size_t  nswp = code.work;
                const size_t *ltab = code.ltab;
                const size_t *rtab = code.rtab;
                for(size_t k=0;k<nswp;++k)
                {
                    typename ARRAY::type &l = arr[ ltab[k] ];
                    typename ARRAY::type &r = arr[ rtab[k] ];
                    if( std::abs(l) < std::abs(r) ) out_of_reach::swap(&l,&r,sizeof(typename ARRAY::type));
                }
            }

        };


        //______________________________________________________________________
        //
        //
        //! assembling swaps into an algorithm
        //
        //______________________________________________________________________
        template <class SWAPS>
        class agenda : public SWAPS, public algorithm
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit agenda() noexcept :
            SWAPS(), algorithm( static_cast<const SWAPS &>(*this) )
            {}

            //! cleanup
            inline virtual ~agenda() noexcept
            {
            }

            //! increasing sort arr[0..code.size-1]
            template <typename T> inline
            void csort(T *arr) const noexcept
            {
                assert(NULL!=arr);
                thin_array<T> ARR(arr,code.size);
                this->increasing(ARR);
            }

            //! increasing co-sort arr[0..code.size-1] and brr[0..code.size-1]
            template <typename T, typename U> inline
            void csort(T *arr, U *brr) const noexcept
            {
                assert(NULL!=arr);
                thin_array<T> ARR(arr,code.size);
                thin_array<U> BRR(brr,code.size);
                this->increasing(ARR,BRR);
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agenda);
        };
    }


}

#endif
