//! \file

#ifndef YACK_CHEMICAL_ENGINE_INCLUDED
#define YACK_CHEMICAL_ENGINE_INCLUDED 1

#include "yack/chem/realm.hpp"
#include "yack/chem/realm/reactor.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //! base class for engine
        //______________________________________________________________________
        typedef cxx_list_of<reactor> engine_;

        //______________________________________________________________________
        //
        //
        //! engine : holding one reactor per domain
        //
        //______________________________________________________________________
        class engine : public engine_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~engine() noexcept;     //!< cleanup
            explicit engine(const realm &); //!< creator reactor(s)

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve top level C and K
            void operator()(const xmlog            &xml,
                            writable<double>       &C,
                            const readable<double> &K);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(engine);
        };

    }
}

#endif

