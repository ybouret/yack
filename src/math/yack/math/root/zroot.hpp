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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit zroot() throw();  //!< setup
            virtual ~zroot() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! main algorithm: initialize and processing
            /**
             f.b = F(x.b) is always the last evaluated value
             */
            bool find(real_function<T>   &F,
                      triplet<T>         &x,
                      triplet<T>         &f) const;

            //! wrapper for any functionoid
            template <typename FUNCTION> inline
            bool operator()(FUNCTION   &F,
                            triplet<T> &x,
                            triplet<T> &f) const
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                return find(FF,x,f);
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zroot);
            //! setup from precomputed (x.a,f.a), (x.c,f.c)
            /**
             - prepare signs
             - recompute F(x.b) in case of 'exact' zero
             */
            result initialize(real_function<T>   &F,
                              triplet<T>         &x,
                              triplet<T>         &f,
                              triplet<sign_type> &s) const;

            //! specific algorithm
            virtual bool processing(real_function<T>   &F,
                                    triplet<T>         &x,
                                    triplet<T>         &f,
                                    triplet<sign_type> &s) const = 0;

        };
        
    }
    
}

#endif

