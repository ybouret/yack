//! \file

#ifndef YACK_JIVE_PATTERN_INCLUDED
#define YACK_JIVE_PATTERN_INCLUDED 1

#include "yack/jive/source.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/data/list/cloneable.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! matching patterns
        //
        //______________________________________________________________________
        class pattern : public object, public counted, public ios::serializable
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~pattern() throw();

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual pattern *clone() const = 0; //!< cloneable interface
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const uint32_t      uuid; //!< identifier
            pattern            *next; //!< for list
            pattern            *prev; //!< for list
            const void * const *self; //!< to retrieve
        protected:

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pattern);
        };

        typedef arc_ptr<const pattern>       motif;    //!< alias
        typedef cloneable_list_of<pattern>   patterns; //!< alias

    }
}

#endif

