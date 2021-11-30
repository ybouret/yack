//! \file

#ifndef YACK_MATH_JACOBIAN_INCLUDED
#define YACK_MATH_JACOBIAN_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! numerical jacobian
        //
        //______________________________________________________________________
        template <typename T>
        class jacobian : public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<T>            tableaux;   //!< alias
            typedef typename tableaux::type array_type; //!< alias
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~jacobian() throw();
            
            //! setup
            explicit jacobian(const size_t ndat, const T = 1e-4) throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! generic computation, with steps regularization
            template <typename FUNCTION, typename INPUT> inline
            void operator()(matrix<T> &J, FUNCTION &f, INPUT &x)
            {
                assert(J.cols==x.size());
                const size_t nvar = x.size();
                const size_t dims = J.rows;
                this->ensure(dims);
                for(size_t j=nvar;j>0;--j)
                {
                    T &xj = coerce(x[j]);
                    T  hp = scal;
                    T  hm = scal;
                    { const temporary<T> _(xj,regp(xj,hp)); f(fp,x); }
                    { const temporary<T> _(xj,regm(xj,hm)); f(fm,x); }
                    const T den = hm+hp;
                    for(size_t i=dims;i>0;--i)
                    {
                        J[i][j] = (fp[i]-fm[i])/den;
                    }
                }
            }
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            T              scal; //!< scaling

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jacobian);
            array_type &fp;
            array_type &fm;
            static T regp(const T xx, T &hh);
            static T regm(const T xx, T &hh);

        };
        
    }
    
}

#endif
