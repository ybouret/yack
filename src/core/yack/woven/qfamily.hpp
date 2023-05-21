
//! \file

#ifndef YACK_WOVEN_QFAMILY_INCLUDED
#define YACK_WOVEN_QFAMILY_INCLUDED 1

#include "yack/woven/qvector.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/data/set.hpp"

namespace yack
{
    namespace woven
    {

        typedef cxx_list_of<qvector>   qvectors; //!< base class
        typedef memory::dyadic         qmemory;  //!< base class
        typedef cxx_array<apq,qmemory> qtableau; //!< helper class
        typedef data_set<size_t>       qindices; //!< indices
        typedef qindices::list_type    indxList;
        typedef qindices::bank_type    indxBank;
        typedef qindices::fund_type    indxFund;

        //______________________________________________________________________
        //
        //
        //! family of orthogonal integer vectors
        //
        //______________________________________________________________________
        class qfamily : public metrics, public qvectors
        {
        public:
            static const char clid[]; //!< "qfamily"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qfamily(const size_t     dims,
                             const size_t     rank,
                             const indxFund  &isrc); //!< create with dimensions
            virtual ~qfamily() noexcept;             //!< cleanup
            qfamily(const qfamily &);                //!< hard copy


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

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
                    return false;
                else
                {
                    load(arr);
                    return contains();
                }
            }

            //! try grow a new vector
            template <typename ARRAY> inline
            bool try_grow(ARRAY &arr, const size_t idx)
            {
                if(accepts(arr)) { grow(idx); return true;  }
                else             { drop(idx); return false; }
            }

            //! append idx to primary
            void grow(const size_t idx);

            //! append idx to replica
            void drop(const size_t idx);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            indxList  code;  //!< list of indices to explore
            qindices  base;  //!< indices of independant vectors
            qindices  deps;  //!< indices of dependent vectors
            qfamily  *next;  //!< for list
            qfamily  *prev;  //!< for list

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            qtableau qarr;
            qtableau qtmp;

            bool accepts();  // from loaded qarr
            bool contains(); // from loaded qarr

            //! helper to load arr into qarr
            template <typename ARRAY>
            void load(ARRAY &arr) {
                assert(dimensions==arr.size());
                for(size_t i=dimensions;i>0;--i) qarr[i] = arr[i];
            }
        };
    }

}

#endif

