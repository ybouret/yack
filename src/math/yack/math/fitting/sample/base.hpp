//! \file

#ifndef YACK_FIT_SAMPLE__INCLUDED
#define YACK_FIT_SAMPLE__INCLUDED 1

#include "yack/math/fitting/variables.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {


            //__________________________________________________________________
            //
            //
            //! sample base type
            //
            //__________________________________________________________________
            class sample_ : public large_object, public counted, public releasable
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const char clid[]; //!< "sample"
                typedef memory::dyadic         allocator; //!< alias;

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                virtual          ~sample_()         throw(); //!< cleanup
                const string    & key() const       throw(); //!< for db
                variables       & operator*()       throw(); //!< internal variables
                const variables & operator*() const throw(); //!< internal variables

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string   name; //!< identifier

            protected:
                //! setup with name
                template <typename ID> inline
                explicit sample_(const ID &id) :
                large_object(),
                counted(),
                name(id),
                vars_()
                {}

                //! throw if multiple names for samples
                void throw_multiple_name(const string &) const;

                //! throw if name is not found
                void throw_unknown_name(const string &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_);
                variables vars_;
            };

            
        }

    }

}

#endif

