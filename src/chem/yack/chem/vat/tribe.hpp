
//! \file

#ifndef YACK_CHEM_VAT_TRIBE_INCLUDED
#define YACK_CHEM_VAT_TRIBE_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef core_repo<const Equilibrium> Tribe_;
        typedef Tribe_::node_type            TribeNode;
        
        class Tribe : public object, public Tribe_
        {
        public:
            explicit Tribe(const Equilibrium &first);  //!< setup with first
            virtual ~Tribe() noexcept;                 //!< cleanup
            Tribe(const Tribe &, const Equilibrium &); //!< copy + append

            bool accepts(const Equilibrium &, const matrix<bool> &) const noexcept;
            YACK_PROTO_OSTREAM(Tribe);
            
            Tribe *next;
            Tribe *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribe);
        };

        typedef cxx_list_of<Tribe> Tribes_;

        class Tribes : public Tribes_
        {
        public:
            explicit Tribes() noexcept;
            virtual ~Tribes() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribes);
        };
    }

}

#endif

