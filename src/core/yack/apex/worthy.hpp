
//! \file

#ifndef YACK_APEX_WORTHY_INCLUDED
#define YACK_APEX_WORTHY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    //! Wary ORTHho Yield
    struct worthy
    {
        static const char * const clid;
        
        typedef cxx_array<apq> qcoeffs;
        
        class qarray : public object
        {
        public:
            template <typename T>
            explicit qarray(const readable<T> &cof) :
            object(), next(0), prev(0), coef(cof,transmogrify), nrm2(0)
            {
                setup( );
            }
            
            qarray(const qarray &other);
            virtual ~qarray() throw();
            
            apq weight(const readable<apq> &v) const;
            
            qarray       *next;
            qarray       *prev;
            const qcoeffs coef;  //!< vector, simplified
            const apn     nrm2;  //!< |coef|^2
            
            friend std::ostream & operator<<(std::ostream &os, const qarray &self)
            {
                os << '|' << self.coef << '|' << '=' << self.nrm2;
                return os;
            }
            
        private:
            YACK_DISABLE_ASSIGN(qarray);
            void setup();
        };
        
        class qfamily : public object
        {
        public:
            const size_t dimension;
            qcoeffs      u_k;
            qcoeffs      v_k;
            
            explicit qfamily(const size_t dims);
            virtual ~qfamily() throw();
            qfamily(const qfamily &);
            
            void free() throw();
            
            template <typename T>
            bool grow(const readable<T> &arr)
            {
                assert(arr.size()==dimension);
                for(size_t i=dimension;i>0;--i)
                     u_k[i] = v_k[i] = arr[i];
                return try_grow_();
            }
            
            const list_of<qarray> * operator->() const throw() { return &U; }
            const list_of<qarray> & operator*()  const throw() { return  U; }
            
        private:
            YACK_DISABLE_ASSIGN(qfamily);
            cxx_list_of<qarray> U;
            
            bool try_grow_();
            
        };
        
        
        
        
    };
}

#endif

