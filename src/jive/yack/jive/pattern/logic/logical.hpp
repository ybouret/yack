//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! logical
        //
        //______________________________________________________________________
        class logical : public pattern, public patterns
        {
        public:
            //! append a new pattern
            logical & operator<<( pattern *p ) throw();

            //! append a new single
            logical & add(const uint8_t);

            //! append a new range
            logical & add(const uint8_t, const uint8_t);

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t serialize(ios::ostream &fp) const;

            static pattern *among(const char *,const  size_t); //!< OR
            static pattern *among(const char *);               //!< OR
            static pattern *among(const string &);             //!< OR

            static pattern *equal(const char *,const size_t); //!< AND
            static pattern *equal(const char *);              //!< AND
            static pattern *equal(const string &);            //!< AND


            static pattern *avoid(const char *,const size_t); //!< NONE
            static pattern *avoid(const char *);              //!< NONE
            static pattern *avoid(const string &);            //!< NONE

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~logical() throw();               //!< cleanup
        protected:
            explicit logical(const uint32_t) throw(); //!< setup
            void     viz(ios::ostream &)       const; //!< render patterns and create links
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(logical);
            static pattern *fill(logical *p, const char *, size_t);
        };


    }

}

#endif
