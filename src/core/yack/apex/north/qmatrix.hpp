//! \file

#ifndef YACK_APEX_NORTH_QMATRIX_INCLUDED
#define YACK_APEX_NORTH_QMATRIX_INCLUDED 1

#include "yack/apex/north/qvector.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/ptr/contractor.hpp"
#include "yack/memory/shelf.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/memory/sentry.hpp"

//! sentry activation
#define YACK_NORTH_SENTRY 1

namespace yack
{
    namespace randomized
    {
        class bits;
    }

    namespace hashing
    {
        class function;
    }

    namespace north
    {

        typedef uint64_t hkey_type;

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
            typedef qvector          qrow;           //!< alias
            typedef readable<qrow>   rd_t;           //!< alias
            typedef rd_t::const_type const_qrow;     //!< alias

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
            // ios::serializable interface
            //__________________________________________________________________
            virtual const char *class_uid()              const throw();
            virtual size_t      serialize(ios::ostream&) const;

            //__________________________________________________________________
            //
            //! display as a julia matrix
            //__________________________________________________________________
            std::ostream & julia(std::ostream &os) const;

            //__________________________________________________________________
            //
            //! getting a hash
            //__________________________________________________________________
            hkey_type hash_with(hashing::function &) const;


            //__________________________________________________________________
            //
            //! univocal Gram-Schmidt algorithm to grow family
            //__________________________________________________________________
            template <typename U> inline
            bool grow(const readable<U> &user)
            {
                assert(user.size()==dimension);
                if(evaluated>=dimension)
                {
                    //----------------------------------------------------------
                    //
                    // full!
                    //
                    //----------------------------------------------------------
                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // use following qvector as apz workspace
                    //
                    //----------------------------------------------------------
#if defined(YACK_NORTH_SENTRY) && (1==YACK_NORTH_SENTRY)
                    YACK_MEM_SENTRY_FOR(obj(),dimension);
#endif
                    const size_t    following = evaluated+1;                // following index
                    const qrow     &brand_new = row[following];             // following vector
                    thin_array<apq> u_k( vgs(), dimension);                 // apq workspace
                    thin_array<apz> v_k( &coerce(brand_new[1]), dimension); // apz workspace
                    for(size_t i=dimension;i>0;--i)
                        u_k[i] = v_k[i] = user[i];

                    //----------------------------------------------------------
                    //
                    // apply algortihm
                    //
                    //----------------------------------------------------------
                    if(qrow::grow(u_k,v_k,*this))
                    {
                        //------------------------------------------------------
                        // success, update
                        //------------------------------------------------------
                        prepare_vector(v_k,u_k, coerce(brand_new.norm2));
                        coerce(situation) = updated_situation(dimension,coerce(evaluated) = following);
                        reschedule();
                        for(size_t i=1;i<evaluated;++i) assert(0 == brand_new.dot(row[i]));
                        return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // failure, user was in current span
                        //------------------------------------------------------
                        return false;
                    }

                }

            }

            //__________________________________________________________________
            //
            //! checking
            //__________________________________________________________________
            template <typename U> inline
            bool includes(const readable<U> &user) 
            {
                assert(user.size()==dimension);
                thin_array<apq> u_k( vgs(), dimension);
                thin_array<apz> v_k( obj(), dimension);

#if defined(YACK_NORTH_SENTRY) && (1==YACK_NORTH_SENTRY)
                YACK_MEM_SENTRY_FOR(idx           , dimension);
                YACK_MEM_SENTRY_FOR(obj(dimension), dimension*dimension);
#endif

                //--------------------------------------------------------------
                //
                // load user data
                //
                //--------------------------------------------------------------
                for(size_t i=dimension;i>0;--i)
                    u_k[i] = v_k[i] = user[i];

                //--------------------------------------------------------------
                //
                // check 
                //
                //--------------------------------------------------------------
                return !qrow::grow(u_k,v_k,*this);
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
            const readable<apz> & last() const throw();

            //__________________________________________________________________
            //
            //! testing equality using indexed vectors
            //__________________________________________________________________
            static bool equality(const qmatrix & , const qmatrix &) throw();

            //! use equality function
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
            contractor<apz>  obj; //!< obj[dimension*dimension+dimension]
            contractor<qrow> row; //!< row[dimension]
            contractor<apq>  vgs; //!< vgs[dimension] for G-S algorithm

            void reschedule() throw();
            void build_copy(const qmatrix &);
            void initialize();
            
        };



    }


}

#endif

