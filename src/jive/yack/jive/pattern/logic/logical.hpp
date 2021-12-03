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
        class logical : public pattern, public motifs
        {
        public:
            //! append a new motif
            logical & operator<<( pattern *p );

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t serialize(ios::ostream &fp) const;

            static pattern *among(const char *,size_t);
            static pattern *among(const char *);
            static pattern *among(const string &);

            static pattern *equal(const char *,size_t);
            static pattern *equal(const char *);
            static pattern *equal(const string &);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~logical() throw();               //!< cleanup
        protected:
            explicit logical(const uint32_t) throw(); //!< setup
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(logical);
        };


    }

}

#endif
