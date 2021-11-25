//! \file

#ifndef YACK_ROOT_ZROOT_INCLUDED
#define YACK_ROOT_ZROOT_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/signs.hpp"

namespace yack
{
    
    namespace math
    {

        namespace core
        {
            //__________________________________________________________________
            //
            //
            //! base class for root finding algorithm
            //
            //__________________________________________________________________
            class zroot
            {
            public:
                virtual            ~zroot()      throw();     //!< cleanup
                virtual const char *name() const throw() = 0; //!< identifier
            protected:                                        //|
                explicit            zroot()      throw();     //!< setup
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(zroot);
            };
        }

        //______________________________________________________________________
        //
        //
        //! base class to setup find algorithm
        //
        //______________________________________________________________________
        template <typename T>
        class zroot : public core::zroot
        {
        public:
            inline virtual ~zroot() throw() {}  //!< cleanup
            inline explicit zroot() throw() : core::zroot() {}  //!< setup
            
        protected:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            //! initialize result
            enum result
            {
                success, //! exact zero
                failure, //! unexpected
                compute  //! need to work
            };

            //! setup from precomputed (x.a,f.a), (x.c,f.c)
            /**
             - prepare signs
             - recompute F(x.b) in case of 'exact' zero
             */
            template <typename FUNCTION>
            inline result initialize(FUNCTION           &F,
                                     triplet<T>         &x,
                                     triplet<T>         &f,
                                     triplet<sign_type> &s) const throw()
            {
                s.a = __sign::of(f.a);
                s.c = __sign::of(f.c);
                switch( __sign::pair(s.a,s.c) )
                {
                    case nn_pair:
                    case pp_pair:
                        return failure;
                        
                    case zp_pair:
                    case zn_pair:
                    case zz_pair:
                        f.b = f.a = F(x.b=x.a);
                        return success;
                        
                    case pz_pair:
                    case nz_pair:
                        f.b = f.c = F(x.b = x.c);
                        return success;
                        
                    case np_pair:
                    case pn_pair:
                        break;
                      
                }
                if(x.a>x.c)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                    cswap(s.a,s.c);
                }
                assert(x.a<=x.c);
                return compute;
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zroot);
        };
        
    }
    
}

#endif

