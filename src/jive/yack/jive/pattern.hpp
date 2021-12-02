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

        class first_bytes;

        //! arguments for accept routine
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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~pattern() throw();

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            //! accept routine
            /**
             * token must be empty on call, and left empty if
             * result it not accepted
             */
            virtual bool   accept(YACK_JIVE_PATTERN_ARGS) const = 0;

            //! true if not empty token are accepted
            virtual bool   strong() const                       = 0;

            //! fill first bytes
            virtual void   firsts(first_bytes &) const          = 0;


            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            bool           feeble() const; //!< !strong(), empty token may be accepted!

            //__________________________________________________________________
            //
            //! reload
            //__________________________________________________________________
            static pattern *load(ios::istream &);

            //__________________________________________________________________
            //
            //! required conversion
            //__________________________________________________________________
            template <typename PATTERN> inline
            PATTERN *as() throw()
            {
                assert(self); assert(uuid==PATTERN::mark); return (PATTERN *)self;
            }

            //__________________________________________________________________
            //
            //! required conversion
            //__________________________________________________________________
            template <typename PATTERN> inline
            const PATTERN *as() const throw()
            {
                assert(self); assert(uuid==PATTERN::mark); return (const PATTERN *)self;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint32_t      uuid; //!< identifier
            pattern            *next; //!< for list
            pattern            *prev; //!< for list
            const void * const *self; //!< to retrieve

        protected:
            explicit pattern(const uint32_t) throw(); //!< setup with uuid, self=0
            void     I_am(const void*) throw();       //!< set self to most derived class
            size_t   emit_uuid(ios::ostream &) const; //!< emit uuid, return written bytes

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

