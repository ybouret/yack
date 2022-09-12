//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fitting/sample_.hpp"
#include "yack/math/fitting/sequential.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! sample interface
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sample : public sample_
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<ABSCISSA,ORDINATE>      sequential_type;    //!< alias
                typedef int (*comparator)(const ABSCISSA &, const ABSCISSA &); //!< alias to create index


                //______________________________________________________________
                //
                //! create a wrapper for a simple function
                //______________________________________________________________
                template <typename FUNC>
                class sequential_wrapper : public sequential_type
                {
                public:
                    inline explicit sequential_wrapper(FUNC &f) throw() : sequential_type(), host(f) {} //!< setup
                    inline virtual ~sequential_wrapper()        throw() {}                              //!< cleanup

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(sequential_wrapper);
                    FUNC &host;
                    inline virtual ORDINATE on_start(const ABSCISSA ini, const readable<ORDINATE> &A, const variables &vars) { return host(ini,A,vars); }
                    inline virtual ORDINATE on_reach(const ABSCISSA, const ABSCISSA end, const readable<ORDINATE> &A, const variables &vars) { return host(end,A,vars); }
                };

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~sample() throw() {}

            protected:

                //! setup with name
                template <typename ID> inline
                explicit sample(const ID &id) :
                sample_(id)
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);

            };

        }
    }
}

#endif

