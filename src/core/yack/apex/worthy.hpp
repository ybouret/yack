
//! \file

#ifndef YACK_APEX_WORTHY_INCLUDED
#define YACK_APEX_WORTHY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
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
            friend std::ostream & operator<<(std::ostream &os, const qarray &self)
            {
                os << '|' << self.coef << '|' << '=' << self.nrm2;
                return os;
            }
            
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
        class qfamily : public object
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
            void release() throw(); //!< release all memory

            
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
            
            
            const list_of<qarray> * operator->() const throw() { return &U; } //!< access
            const list_of<qarray> & operator*()  const throw() { return  U; } //!< access
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimension; //!< space dimension

            
        private:
            YACK_DISABLE_ASSIGN(qfamily);
            qcoeffs             u_k;       //!< workspace
            qcoeffs             v_k;       //!< workspace
            cxx_list_of<qarray> U;         //!< current list of vectors
            
            bool try_grow();
            
        };
        
        
        
        
    };
}

#endif

