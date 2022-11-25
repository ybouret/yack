
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

            bool compile() throw();

            //! apply restriction
            /**
             target <- source
             */
            double apply(writable<double>       &target,
                         const readable<double> &source,
                         raddops                &xadd) const;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(restriction);
            const unsigned q2; //!< sum of squared coefficients
        };

    }
}

#endif
