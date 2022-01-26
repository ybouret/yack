//! \file

#ifndef YACK_ROOT_ZROOT_INCLUDED
#define YACK_ROOT_ZROOT_INCLUDED 1

#include "yack/math/triplet.hpp"
#include "yack/math/real-function.hpp"
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
                //______________________________________________________________
                //
                // types
                //______________________________________________________________

                //! result type
                enum result
                {
                    success, //! (almost) exact zero
                    failure, //! unexpected
                    compute  //! need to work
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
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
            explicit zroot() throw();  //!< setup
            virtual ~zroot() throw(); //!< cleanup


            bool find(real_function<T>   &F,
                      triplet<T>         &x,
                      triplet<T>         &f) const
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                triplet<sign_type> s = { __zero__, __zero__, __zero__ };
                switch( initialize(F,x,f,s) )
                {
                    case failure: return false;
                    case success: return true;
                    case compute: break;
                }

                //______________________________________________________________
                //
                // processing
                //______________________________________________________________
                assert(x.a<=x.c);
                assert(s.a!=__zero__);
                assert(s.c!=__zero__);
                assert(s.a!=s.c);

                return processing(F,x,f,s);
            }

            template <typename FUNCTION> inline
            bool operator()(FUNCTION   &F,
                            triplet<T> &x,
                            triplet<T> &f) const
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                return find(FF,x,f);
            }


        protected:

            //! setup from precomputed (x.a,f.a), (x.c,f.c)
            /**
             - prepare signs
             - recompute F(x.b) in case of 'exact' zero
             */
            result initialize(real_function<T>   &F,
                              triplet<T>         &x,
                              triplet<T>         &f,
                              triplet<sign_type> &s) const;

            //! wrapper for initialize
            template <typename FUNCTION>
            inline result initialize_for(FUNCTION           &F,
                                         triplet<T>         &x,
                                         triplet<T>         &f,
                                         triplet<sign_type> &s) const
            {

                typename real_function_of<T>::template call<FUNCTION> FF(F);
                return initialize(FF,x,f,s);
            }

            virtual bool processing(real_function<T> &F,
                                    triplet<T> &x,
                                    triplet<T> &f,
                                    triplet<sign_type> &s) const = 0;


            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zroot);
        };
        
    }
    
}

#endif

