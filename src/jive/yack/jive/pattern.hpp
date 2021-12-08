//! \file

#ifndef YACK_JIVE_PATTERN_INCLUDED
#define YACK_JIVE_PATTERN_INCLUDED 1

#include "yack/jive/source.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/ios/gv/vizible.hpp"
#include "yack/data/list/cloneable.hpp"

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
        class pattern : public object, public counted, public ios::serializable, public ios::vizible
        {
        public:
            static bool verbose; //!< activate tracing
            
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

            //! collect first bytes
            virtual void   firsts(first_bytes &) const          = 0;

            //! clone
            virtual pattern *clone()             const          = 0;

            //! encode as GraphViz
            virtual void   encode(ios::ostream &) const         = 0;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            bool           feeble() const; //!< !strong(), empty token may be accepted!
            void           saveGV(ios::ostream &) const; //!< encode with prolog/epilog
            void           makeGV(const string &) const; //!< save/render
            void           makeGV(const  char  *) const; //!< save/render

            //__________________________________________________________________
            //
            //! reload
            //__________________________________________________________________
            static pattern *load(ios::istream &);

            //__________________________________________________________________
            //
            //! build the ignore case version
            //__________________________________________________________________
            static pattern *ignore_case(pattern *p);

            //__________________________________________________________________
            //
            //! express as string
            //__________________________________________________________________
            static void     express(string &target, const pattern *p);
            string          express() const; //!< express as string


            //__________________________________________________________________
            //
            //! equality
            //__________________________________________________________________
            static bool are_same(const pattern &lhs, const pattern &rhs) throw();

            //! wrapper
            friend bool operator==(const pattern &lhs, const pattern &rhs) throw();

            //! wrapper
            friend bool operator!=(const pattern &lhs, const pattern &rhs) throw();

            //__________________________________________________________________
            //
            //! optimize
            //__________________________________________________________________
            static pattern *optimize(pattern *);

            //__________________________________________________________________
            //
            //! check conversion
            //__________________________________________________________________
            template <typename PATTERN> inline
            bool is() const throw() { return PATTERN::mark == uuid; }

            //__________________________________________________________________
            //
            //! required conversion
            //__________________________________________________________________
            template <typename PATTERN> inline
            PATTERN *as() throw() { assert(self); assert(uuid==PATTERN::mark); return (PATTERN *)self; }

            //__________________________________________________________________
            //
            //! required conversion
            //__________________________________________________________________
            template <typename PATTERN> inline
            const PATTERN *as() const throw() { assert(self); assert(uuid==PATTERN::mark); return (const PATTERN *)self; }


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

        typedef arc_ptr<const pattern>      motif;    //!< alias
        typedef cloneable_list_of<pattern>  patterns; //!< alias
    }
}

//! trace working
#define YACK_JIVE_PRINTLN(MSG)  do { if(jive::pattern::verbose) { std::cerr << MSG << std::endl; } } while(false)



#endif

