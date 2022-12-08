
//! \file

#ifndef YACK_APEX_WORTHY_INCLUDED
#define YACK_APEX_WORTHY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! Wary ORTHho Yield
    //
    //__________________________________________________________________________
    struct worthy
    {


        typedef memory::dyadic memory_model;

        //______________________________________________________________________
        //
        //! tableau of rational
        //______________________________________________________________________
        typedef cxx_array<apq,memory_model> coefficients;



        //______________________________________________________________________
        //
        //! array of coefficients with its norm
        //______________________________________________________________________
        class qarray : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
           
            //! setup : load and simplify coefficients, compute nrm2
            template <typename T>
            explicit qarray(const readable<T> &cof) :
            object(), next(0), prev(0), coef(cof,transmogrify), nrm2(0) {
                setup( );
            }
            
            //! cleanup
            virtual ~qarray() throw();

            //! hard copy (for list copy constructor)
            qarray(const qarray &other);
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! compute Gram-Schmidt weight = <coef|v>/nrm2
            apq weight(const readable<apq> &v) const;
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const qarray &);

            //! test all coefficients equality
            friend bool operator==(const qarray &lhs, const qarray &rhs) throw();

            //! test coefficients difference
            friend bool operator!=(const qarray &lhs, const qarray &rhs) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            qarray            *next;  //!< for list
            qarray            *prev;  //!< for list
            const coefficients coef;  //!< coefficients, simplified
            const apn          nrm2;  //!< |coef|^2

        private:
            YACK_DISABLE_ASSIGN(qarray);
            void setup();
        };


        //! family maturity
        enum maturity
        {
            in_progress, //! size < dims-1
            almost_done, //! size == dims-1, next vector is unique!
            fully_grown  //! size == dims
        };

        static maturity maturity_of(const size_t dims, const size_t size) throw();

        //______________________________________________________________________
        //
        //! family of simplified, univocal vectors
        //______________________________________________________________________
        class qfamily  
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qfamily(const size_t dims); //!< setup with space dimension
            virtual ~qfamily() throw();          //!< cleanup
            qfamily(const qfamily &);            //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset() throw(); //!< release/free all memory

            
            //! grow algorithm
            /**
             - apply Gram-Schmidt, returns as soon as possible
             - all the vectors are univocal
             */
            template <typename T>
            bool grow(const readable<T> &arr)
            {
                assert(arr.size()==dimension);
                for(size_t i=dimension;i>0;--i)
                     u_k[i] = v_k[i] = arr[i];
                return try_grow();
            }

            const list_of<qarray> * operator->()  const throw() { return &U; } //!< access
            const list_of<qarray> & operator*()   const throw() { return  U; } //!< access

            //! display
            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            static bool eq(const qfamily &lhs, const qfamily &rhs) throw();

            //! check same families
            friend bool operator==(const qfamily &lhs, const qfamily &rhs) throw();

            //! return simplified projected vector
            /**
             should be the identity for any vector
             in the span of the current basis
             */
            template <typename T>
            const readable<apq> &project(const readable<T> &vec)
            {
                assert(vec.size()==dimension);
                for(size_t i=dimension;i>0;--i)
                {
                    v_k[i] = vec[i];
                    u_k[i].ldz();
                }
                return project();
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t       dimension; //!< space dimension
            const maturity     situation; //!< current maturity

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            coefficients              u_k;       //!< workspace
            coefficients              v_k;       //!< workspace
            cxx_list_of<qarray>       U;         //!< current list of vectors
            cxx_series<coefficients*> Q;         //!< pointer to vectors
            cxx_series<size_t>        I;         //!< current indices

            bool try_grow();
            const readable<apq> &project();
            
        };
        
        
        
        
    };
}

#endif

