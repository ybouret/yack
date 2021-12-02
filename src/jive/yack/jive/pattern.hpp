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

#define YACK_JIVE_PATTERN_ARGS source &src, token &tkn

        //______________________________________________________________________
        //
        //
        //! matching patterns
        //
        //______________________________________________________________________
        class pattern : public object, public counted, public ios::serializable
        {
        public:
            enum result
            {
                accepted, //!< accepted
                rejected, //!< rejected
                finished  //!< end of stream
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~pattern() throw();

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual result accept(YACK_JIVE_PATTERN_ARGS) const = 0;


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
            void     I_am(const void*) throw();       //!< set self to most derived class
            size_t   emit_uuid(ios::ostream &) const;

            //! helper
            template <typename T> inline
            void I_am() throw() { I_am( static_cast<const T*>(this) ); }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pattern);
        };

        typedef linked_ptr<const pattern> motif;    //!< alias
        typedef cxx_list_of<motif>        motifs;   //!< alias
        
    }
}

#endif

