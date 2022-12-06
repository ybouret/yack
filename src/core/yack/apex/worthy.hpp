
//! \file

#ifndef YACK_APEX_WORTHY_INCLUDED
#define YACK_APEX_WORTHY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/list/cxx.hpp"

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
        
        //______________________________________________________________________
        //
        //! array of rational coefficients
        //______________________________________________________________________
        typedef cxx_array<apq> qcoeffs;



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
           
            //! setup 
            template <typename T>
            explicit qarray(const readable<T> &cof) :
            object(), next(0), prev(0), coef(cof,transmogrify), nrm2(0)
            {
                setup( );
            }
            
            //! cleanup
            virtual ~qarray() throw();

            //! copy (for list copy)
            qarray(const qarray &other);
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! compute Gram-Schmidt weight
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
            qarray       *next;  //!< for list
            qarray       *prev;  //!< for list
            const qcoeffs coef;  //!< coefficients, should be simplified
            const apn     nrm2;  //!< |coef|^2



        private:
            YACK_DISABLE_ASSIGN(qarray);
            void setup();
        };


        
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

            bool                    fully_grown() const throw(); //!< size >= dims
            const list_of<qarray> * operator->()  const throw() { return &U; } //!< access
            const list_of<qarray> & operator*()   const throw() { return  U; } //!< access

            //! display
            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            //! check same families
            friend bool operator==(const qfamily &lhs, const qfamily &rhs) throw();
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t       dimension; //!< space dimension

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            qcoeffs              u_k;       //!< workspace
            qcoeffs              v_k;       //!< workspace
            cxx_list_of<qarray>  U;         //!< current list of vectors
            cxx_series<qcoeffs*> Q;         //!< pointer to vectors
            cxx_series<size_t>   I;         //!< current indices

            bool try_grow();
            
            

            
        };
        
        
        
        
    };
}

#endif

