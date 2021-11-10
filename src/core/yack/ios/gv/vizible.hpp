//! \file

#ifndef YACK_VIZIBLE_INCLUDED
#define YACK_VIZIBLE_INCLUDED 1

#include "yack/ios/ostream.hpp"

namespace yack
{

    namespace ios
    {

        class vizible
        {
        public:
            virtual ~vizible() throw();

            static  ostream& uuid(ostream &, const void *);

            template <typename NAME> static inline
            ostream & digraph_init(ostream &os, const NAME &name)
            {
                os << "digraph " << name << '{' << '\n';
                return os;
            }

            static void digraph_quit(ostream &os);



        protected:
            explicit vizible() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(vizible);
        };

    }

}

#endif
