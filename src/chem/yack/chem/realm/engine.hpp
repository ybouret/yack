//! \file

#ifndef YACK_CHEMICAL_ENGINE_INCLUDED
#define YACK_CHEMICAL_ENGINE_INCLUDED 1

#include "yack/chem/realm.hpp"
#include "yack/chem/realm/reactor.hpp"

namespace yack
{
    namespace chemical
    {

        typedef cxx_list_of<reactor> engine_;
        
        class engine : public engine_
        {
        public:
            virtual ~engine() noexcept;
            explicit engine(const realm &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(engine);
        };

    }
}

#endif

