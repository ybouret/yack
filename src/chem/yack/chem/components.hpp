
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/component.hpp"

namespace yack
{

    namespace chemical
    {

        class library;
        typedef suffix_set<string,component::pointer> components_;

        class components : public components_
        {
        public:
            virtual ~components() throw();
            explicit components() throw();

            void operator()(const string &expr, library &lib);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
        };
    }

}

#endif
