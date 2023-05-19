
//! \file

#ifndef YACK_WOVEN_QFAMILY_INCLUDED
#define YACK_WOVEN_QFAMILY_INCLUDED 1

#include "yack/woven/qvector.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace woven
    {

        typedef cxx_list_of<qvector>   qvectors;
        typedef memory::dyadic         qmemory;
        typedef cxx_array<apq,qmemory> qtableau;

        class qfamily : public metrics, public qvectors
        {
        public:
            explicit qfamily(const size_t dims);
            virtual ~qfamily() noexcept;
            qfamily(const qfamily &);

            qtableau qarr;
            qtableau qtmp;

            //! accepts a new vector to expand
            template <typename ARRAY>
            bool accepts(ARRAY &arr)
            {
                assert(arr.size()==dimensions);
                load(arr);
                if(size<=0)
                {
                    return q_array::check_not_null(qarr);
                }
                else
                {
                    return accepts();
                }
            }

            //! check if vector is contained in image
            template <typename ARRAY>
            bool contains(ARRAY &arr)
            {
                assert(arr.size()==dimensions);
                if(size<=0)
                {
                    return false;
                }
                else
                {
                    load(arr);
                    return contains();
                }
            }

            template <typename ARRAY> inline
            bool try_grow(ARRAY &arr)
            {
                if(accepts(arr)) {
                    grow();
                    return true;
                }
                else
                {
                    return false;
                }
            }

            void grow();




            qfamily *next;
            qfamily *prev;

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            bool accepts();  // from loaded qarr
            bool contains(); // from loaded qarr

            template <typename ARRAY>
            void load(ARRAY &arr)
            {
                assert(dimensions==arr.size());
                for(size_t i=dimensions;i>0;--i) qarr[i] = arr[i];
            }
        };
    }

}

#endif

