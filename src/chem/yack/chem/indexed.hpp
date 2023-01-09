//! \file

#ifndef YACK_CHEMICAL_INDEXED_INCLUDED
#define YACK_CHEMICAL_INDEXED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace chemical
    {

        class indexed
        {
        public:
            explicit indexed(const size_t) throw(); //!< with positive index
            virtual ~indexed() throw();             //!< cleanup
            indexed(const indexed &) throw();       //!< copy

            size_t operator*() const throw();

            

        private:
            const size_t indx_;
            YACK_DISABLE_ASSIGN(indexed);
        };

    }

}

#endif
