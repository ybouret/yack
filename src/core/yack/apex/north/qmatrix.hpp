//! \file

#ifndef YACK_APEX_NORTH_QMATRIX_INCLUDED
#define YACK_APEX_NORTH_QMATRIX_INCLUDED 1

#include "yack/apex/north/qvector.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/ptr/contractor.hpp"
#include "yack/memory/shelf.hpp"

namespace yack
{
    namespace randomized
    {
        class bits;
    }

    namespace north
    {

        struct qmatrices;

        //______________________________________________________________________
        //
        //
        //! matrix of orthogonal vector(s)
        //
        //______________________________________________________________________
        class qmatrix : public object, public qmetrics, public readable<qvector>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef qvector                   qrow;           //!< alias
            typedef readable<qrow>            rd_t;           //!< alias
            typedef typename rd_t::const_type const_qrow;     //!< alias
            static  const size_t              extra = 2;      //!< extra arrays

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qmatrix(const size_t dims); //!< default construction based on positive dimension
            virtual ~qmatrix()          throw(); //!< cleanup
            qmatrix(const qmatrix &);            //!< hard copy
            
            
            //__________________________________________________________________
            //
            // cloning
            //__________________________________________________________________
            qmatrix *clone() const; //!< return new(*this)
            
            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________

            //! current size = evaluated = rank of the family
            virtual size_t size() const throw();

            //! access components [1:evaluated]
            virtual const_qrow & operator[](const size_t) const throw();

            //__________________________________________________________________
            //
            //! univocal Gram-Schmidt algorithm to grom family
            //__________________________________________________________________
            template <typename U> inline
            bool grow(const readable<U> &user)
            {
                assert(user.size()==dimension);

                apq            *qgs = obj();
                thin_array<apq> u_k( qgs,           dimension );
                thin_array<apq> v_k( qgs+dimension, dimension );

                //--------------------------------------------------------------
                //
                // load user data
                //
                //--------------------------------------------------------------
                for(size_t i=dimension;i>0;--i)
                    u_k[i] = v_k[i] = user[i];

                
                //--------------------------------------------------------------
                //
                // try to grow
                //
                //--------------------------------------------------------------
                if(qrow::grow(u_k,v_k,*this))
                {
                    //----------------------------------------------------------
                    //
                    // Gram-Schmidt succeeded, try to finalize vector
                    //
                    //----------------------------------------------------------
                    return complement(u_k);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // in span or nil vector
                    //
                    //----------------------------------------------------------
                    return false;
                }
            }

            //__________________________________________________________________
            //
            //! human friendly display
            //__________________________________________________________________
            friend std::ostream & operator<<( std::ostream &, const qmatrix &);

            //__________________________________________________________________
            //
            //! Knuth shuffle, mostly to debug
            //__________________________________________________________________
            void shuffle(randomized::bits &ran) throw();

            //__________________________________________________________________
            //
            //! access last inserted
            //__________________________________________________________________
            const readable<apq> & last() const throw();

            //__________________________________________________________________
            //
            //! testing equality using indexed vectors
            //__________________________________________________________________
            static bool equality(const qmatrix & , const qmatrix &) throw();
            friend bool operator==(const qmatrix & , const qmatrix &) throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const maturity situation; //!< current situation
            const size_t   evaluated; //!< number of members in the matrix

        private:
            YACK_DISABLE_ASSIGN(qmatrix);
            friend struct qmatrices;

            size_t          *idx; //!< idx[dimension]
            memory::shelf    lib; //!< linear memory
            contractor<apq>  obj; //!< obj[dimension*dimension+exta*dimension]
            contractor<qrow> row; //!< row[dimension]
            
            void reschedule() throw();
            bool complement(writable<apq> &u_k);
            void build_copy(const qmatrix &);
            void initialize();
            
        };

        //______________________________________________________________________
        //
        //
        //! operations on qmatrices
        //
        //______________________________________________________________________
        struct qmatrices
        {


            //! test equality of last insertion
            static
            bool have_same_last(const qmatrix &lhs,
                                const qmatrix &rhs) throw()
            {
                assert(lhs.dimension==rhs.dimension);
                assert(lhs.evaluated==rhs.evaluated);
                assert(lhs.evaluated>0);
                const size_t size = lhs.evaluated;

#ifndef NDEBUG
                for(size_t i=1;i<size;++i) {
                    assert( comparison::equality(lhs[i],rhs[i]) );
                }
#endif
                return comparison::equality(lhs[size],rhs[size]);

            }
            


        };




    }


}

#endif

