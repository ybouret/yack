//! \file

#ifndef YACK_JIVE_PATTERN_INCLUDED
#define YACK_JIVE_PATTERN_INCLUDED 1

#include "yack/jive/source.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/ptr/linked.hpp"

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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const uint32_t      uuid; //!< identifier
            pattern            *next; //!< for list
            pattern            *prev; //!< for list
            const void * const *self; //!< to retrieve
        protected:
            explicit pattern(const uint32_t) throw(); //!< setup with uuid, self=0
            void I_am(void*) throw();                 //!< set self to most derived class

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pattern);
        };

        typedef linked_ptr<const pattern> motif;    //!< alias
        typedef cxx_list_of<motif>        motifs;   //!< alias
        
    }
}

#endif

