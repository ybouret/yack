
#ifndef YACK_CHEMICAL_RESTRICTION_INCLUDED
#define YACK_CHEMICAL_RESTRICTION_INCLUDED 1

#include "yack/chem/actors.hpp"

namespace yack
{
    class xmlog;

    namespace chemical
    {

        //! restriction on positive concentration
        class restriction : public object, public counted, public actors
        {
        public:
            explicit restriction() throw();
            virtual ~restriction() throw();

            void compile() throw();

            //! apply restriction
            /**
             target <- source
             */
            void apply(writable<double>       &target,
                       const readable<double> &source);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(restriction);
        };

    }
}

#endif
