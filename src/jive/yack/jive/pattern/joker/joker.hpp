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

            


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~joker() throw();                              //!< cleanup

        protected:
            explicit joker(const uint32_t,pattern*) throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(joker);
            const arc_ptr<const pattern> jk;
        };

    }
}

#endif

