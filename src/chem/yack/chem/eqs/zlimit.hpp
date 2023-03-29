//! \file

#ifndef YACK_CHEMICAL_ZLIMIT_INCLUDED
#define YACK_CHEMICAL_ZLIMIT_INCLUDED 1

#include "yack/chem/eqs/actors.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! Zeroing Limit
        //
        //______________________________________________________________________
        class zlimit : public sp_repo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit zlimit(const sp_proxy &) noexcept; //!< setup
            virtual ~zlimit()                 noexcept; //!< cleanup
            zlimit(const zlimit &other);                //!< copy
            zlimit & operator=(const zlimit &);         //!< assign

            //! setup with first species/extent
            explicit zlimit(const sp_proxy &pp,
                            const double    xi,
                            const species  &sp);



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     reset()         noexcept;  //!< extent=0 and clear()
            void     trade(zlimit &) noexcept;  //!< no-throw exchange
            YACK_PROTO_OSTREAM(zlimit);         //!< display

            //! nullify all register top_level species
            void     nullify(writable<double> &C) const noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            double extent; //!<  extent zeroing species

        };





    }

}

#endif

