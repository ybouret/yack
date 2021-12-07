//! \file

#ifndef YACK_JIVE_PATTERN_JOKER_INCLUDED
#define YACK_JIVE_PATTERN_JOKER_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! joker
        //
        //______________________________________________________________________
        class joker : public pattern
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const pattern & operator*() const throw(); //!< get joker
            virtual void firsts(first_bytes &) const;  //!< forward to internal

            void substitute(pattern *) throw();        //!< subsitute pattern

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~joker() throw();                        //!< cleanup

        protected:
            explicit joker(const uint32_t,pattern*) throw(); //!< setup
            explicit joker(const joker &)           throw(); //!< copy
            void     viz(ios::ostream &) const;              //!< render joker and link


        private:
            YACK_DISABLE_ASSIGN(joker);
            const motif jk;
        };

    }
}

#endif

