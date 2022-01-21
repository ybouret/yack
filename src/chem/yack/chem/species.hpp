//! \file

#include "yack/string.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! names species
        //
        //______________________________________________________________________
        class species
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() throw(); //!< cleanup

            //! setup with name+algebraic charge
            template <typename NAME> inline
            explicit species(const NAME &user_n, const int user_z) :
            name(user_n), z(user_z)
            {
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;   //!< unique name
            const int    z;      //!< algebraic charge


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };

    }

}
