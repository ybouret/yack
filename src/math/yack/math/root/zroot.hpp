//! \file

#ifndef YACK_ROOT_ZROOT_INCLUDED
#define YACK_ROOT_ZROOT_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/signs.hpp"

namespace yack
{
    
    namespace math
    {
        
        class zroot
        {
        public:
            triplet<sign_type> s;
            
            enum result
            {
                success, //! exact zero
                failure, //! unexpected
                compute  //! need to work
            };
            
            virtual ~zroot() throw();
            
        protected:
            explicit zroot() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zroot);
        };
        
        template <typename T>
        class zalgo : public zroot
        {
        public:
            inline virtual ~zalgo() throw() {}
            inline explicit zalgo() throw() : zroot() {}
            
            inline
            result initialize( triplet<T> &x, triplet<T> &f)
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
                        x.b = x.a;
                        f.b = f.a;
                        return success;
                        
                    case pz_pair:
                    case nz_pair:
                        x.b = x.c;
                        f.b = f.c;
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
            YACK_DISABLE_COPY_AND_ASSIGN(zalgo);
        };
        
    }
    
}

#endif

