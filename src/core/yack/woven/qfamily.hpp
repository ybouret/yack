
//! \file

#ifndef YACK_WOVEN_QFAMILY_INCLUDED
#define YACK_WOVEN_QFAMILY_INCLUDED 1

#include "yack/woven/qvector.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/data/set.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace woven
    {

        typedef cxx_list_of<qvector>         qvectors; //!< base class
        typedef memory::dyadic               qmemory;  //!< base class
        typedef cxx_array<apq,qmemory>       qtableau; //!< helper class


        struct indices
        {
            typedef data_set<size_t>             set;  //!< indices
            typedef data_set<size_t> ::list_type list;
            typedef data_set<size_t> ::bank_type bank;
            typedef data_set<size_t> ::fund_type fund;
        };

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

            //! initialize family
            /**
             \param dims phase space dimension
             \param rmax maximum rank
             */
            explicit qfamily(const size_t         dims,
                             const indices::fund &fund);
            virtual ~qfamily() noexcept;                 //!< cleanup
            qfamily(const qfamily &);                    //!< hard copy


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            void initialize(const size_t r,
                            const size_t i);

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

            template <typename T> inline
            bool try_grow(const matrix<T> &M)
            {
                assert(indx.size>0);
                const size_t i = indx.pull_head(); assert(i>=1); assert(i<=M.rows);
                return try_grow(M[i],i);
            }

            

            friend std::ostream & operator<<(std::ostream &, const qfamily &);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            indices::list indx;  //!< available indices
            indices::set  base;  //!< indices of independant vectors
            indices::set  deps;  //!< indices of dependent vectors
            qfamily      *next;  //!< for list
            qfamily      *prev;  //!< for list

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            qtableau qarr;
            qtableau qtmp;

            bool accepts();  // from loaded qarr
            bool contains(); // from loaded qarr


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

