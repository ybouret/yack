//! \file

#ifndef YACK_FITTING_REPLICA_INCLUDED
#define YACK_FITTING_REPLICA_INCLUDED 1


#include "yack/math/fitting/primary.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! replica variable = name+primary
            //
            //__________________________________________________________________
            class replica : public variable
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit replica(const string &, const primary::handle &); //!< setup name+link
                virtual ~replica() throw();                                //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                virtual const string & alias()      const throw(); //!< link->name
                virtual bool           is_primary() const throw(); //!< false

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(replica);
                const primary::handle link;
                virtual size_t indx() const throw(); //!< return indx_
            };

        }

    }

}



#endif

