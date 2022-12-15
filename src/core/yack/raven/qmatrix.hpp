//! \file

#ifndef YACK_RAVEN_QMATRIX_INCLUDED
#define YACK_RAVEN_QMATRIX_INCLUDED 1

#include "yack/raven/qvector.hpp"
#include "yack/memory/shelf.hpp"
#include "yack/ptr/contractor.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/memory/sentry.hpp"

#if 1
//! helper to check untouched memory
#define YACK_RAVEN_SENTRY() YACK_MEM_SENTRY_FOR( obj()+dimension, current_rank*dimension);
#else
#define YACK_RAVEN_SENTRY()
#endif

namespace yack
{
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        //! matrix of incremental orthogonal, univocal vectors
        //
        //______________________________________________________________________
        class qmatrix : public object, public qmetrics, public readable<qvector>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~qmatrix() throw();                         //!< cleanup
            explicit qmatrix(const size_t sz, const size_t rk); //!< build with sz>=rk>=2
            qmatrix(const qmatrix &other);                      //!< full copy

            //__________________________________________________________________
            //
            // interface: readable
            //__________________________________________________________________
            virtual size_t         size()                   const throw(); //!< current_rank
            virtual const qvector &operator[](const size_t) const throw(); //!< [1..current_rank]

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t                 allocated()              const throw();      //!< linear memory
            qmatrix               *clone()                          const;      //!< new qmatrix(*this)
            void                   reset()                        throw();      //!< reset
            friend std::ostream   &operator<<(std::ostream &, const qmatrix &); //!< display as matrix

            //! try to insert a new vector
            template <typename T> inline
            bool operator()(const readable<T> &v)
            {
                YACK_RAVEN_SENTRY();
                assert(dimension==v.size());

                thin_array<apz> v_k( obj(), dimension);
                thin_array<apq> u_k( vgs(), dimension);
                for(size_t i=dimension;i>0;--i) {
                    u_k[i] = v_k[i] = v[i];
                }

                return build_next(u_k,v_k);
            }

            //! check if the vector is included in linear space
            template <typename T> inline
            bool includes(const readable<T> &v)
            {
                YACK_RAVEN_SENTRY();
                assert(dimension==v.size());
                thin_array<apz> v_k( obj(), dimension);
                thin_array<apq> u_k( vgs(), dimension);
                for(size_t i=dimension;i>0;--i) {
                    u_k[i] = v_k[i] = v[i];
                }
                keep_ortho(u_k,v_k);
                return is_nil_vec(u_k);
            }


            const size_t maximum_rank; //!< maximum rank
            const size_t current_rank; //!< current rank

        private:
            YACK_DISABLE_ASSIGN(qmatrix);

            memory::shelf       lib; //!< all linear memory
            contractor<apz>     obj; //!< maximum_rank * dimension + dimension for G-S
            contractor<qvector> row; //!< maximum_rank
            contractor<apq>     vgs; //!< dimension for G-S
            
            void initialize();
            void keep_ortho(writable<apq>       &u_k,
                            const readable<apz> &v_k);

            bool is_nil_vec(const readable<apq> &u_k) const throw();

            bool build_next(writable<apq>       &u_k,
                            const readable<apz> &v_k);
        };

    }

}

#endif
