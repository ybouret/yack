
//! \file

#ifndef YACK_CHEM_VAT_LIMIT_INCLUDED
#define YACK_CHEM_VAT_LIMIT_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Limit: value and vanishing species
        //
        //______________________________________________________________________
        class Limit : public Extended::Real, public Species::CoopRepo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Limit(const Species::Fund &) noexcept; //!< setup
            virtual ~Limit() throw();                       //!< cleanup
            Limit(const Limit &);                           //!< copy
            Limit & operator=(const Limit &);               //!< assign

            void neg() noexcept;

            template <typename ARRAY> inline
            void nullify(ARRAY &arr, const IndexLevel level) const
            {
                const typename ARRAY::const_type _0(0);
                for(const Species::Node *sn=head;sn;sn=sn->next)
                {
                    arr[ (***sn).indx[level] ] = _0;
                }
            }


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual void initialize() noexcept; //!< set to 0 and empty
            YACK_PROTO_OSTREAM(Limit);          //!< display
        };

    }

}

#endif

