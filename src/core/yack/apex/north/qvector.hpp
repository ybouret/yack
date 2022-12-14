
//! \file

#ifndef YACK_APEX_NORTH_QVECTOR_INCLUDED
#define YACK_APEX_NORTH_QVECTOR_INCLUDED 1

#include "yack/apex/north/qmetrics.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    namespace north
    {

        //______________________________________________________________________
        //
        //
        //! component of an orthogonal family
        //
        //______________________________________________________________________
        class qvector : public qmetrics, public readable<apz>
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qvector(apz *ptr, const size_t num); //!< setup from initialized ptr[num]
            virtual ~qvector() throw();                   //!< cleanup assigned space

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t       size()                  const throw(); //!< size = matrix.dimension
            virtual const_type & operator[](size_t indx) const throw(); //!< access

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void xch(qvector &other) throw();        //!< no-throw exchange
            apz  dot(const readable<apz> &v) const;  //!<  compute Gram-Schmidt weight = <coeff|v> 
            
            
            //__________________________________________________________________
            //
            //! construct in u_k the orthogonal version of v_k
            /**
             - using Gram-Schmidt direct algorithm
             - no further simplification for u_k
             */
            //__________________________________________________________________
            static bool grow(writable<apq>           &u_k,
                             const readable<apz>     &v_k,
                             const readable<qvector> &U);

            //__________________________________________________________________
            //
            //! human friendly display
            //__________________________________________________________________
            friend std::ostream & operator<<( std::ostream &, const qvector &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
            apz          *coeff;

        public:
            const apn     norm2; //!< current squared norm

            
        };
    }

}

#endif
