
//! \file

#ifndef YACK_CHEMICAL_GATHERING_INCLUDED
#define YACK_CHEMICAL_GATHERING_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/latch.hpp"

namespace yack
{
    namespace chemical
    {

        class gathering : public latch
        {
        public:
            virtual ~gathering() throw();
            explicit gathering() throw();

            virtual size_t size() const throw() = 0;

            void absorb(const entity &) throw(); //!< update maxlen
            std::ostream & pad(std::ostream &os, const entity &) const;

            const size_t maxlen; //!< from entities


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gathering);
        };

    }

}

#endif

