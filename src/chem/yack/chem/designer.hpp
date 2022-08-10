
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/singleton.hpp"
#include "yack/jive/parser.hpp"

namespace yack
{
    namespace chemical
    {

        class designer : public singleton<designer>
        {
        public:
            static const char * const       call_sign; //!< "chemical::designer"
            static const at_exit::longevity life_time = 7777;

            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(designer);
            friend class singleton<designer>;

            explicit designer();
            virtual ~designer() throw();

            auto_ptr<jive::parser> sp;


        };
    }

}

#endif

