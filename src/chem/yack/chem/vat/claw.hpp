//! \file

#ifndef YACK_CHEM_VAT_CLAW_INCLUDED
#define YACK_CHEM_VAT_CLAW_INCLUDED 1

#include "yack/chem/eqs/actors.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/ipso/add.hpp"
#include "yack/data/dinky/core-list.hpp"

namespace yack
{
    namespace Chemical
    {

        typedef ipso::add<double>  Adder; //!< alias
        typedef Adder::inside_type XReal; //!< alias

        //! conversion from double to extended
        inline XReal  Double2XReal(const double x) { return ipso::inside<double>::send(x); }

        //! conversion from extended to double
        inline double XReal2Double(const XReal &x) { return ipso::inside<double>::recv(x); }

        typedef core_list<XReal>     CoreXList; //!< alias
        typedef CoreXList::node_type CoreXNode; //!< alias

        //______________________________________________________________________
        //
        //
        //! Conservation Law as a list of Actors
        //
        //______________________________________________________________________
        class ConservationLaw : public Actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ConservationLaw();          //!< setup
            virtual ~ConservationLaw() noexcept; //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            bool involves(const Species         &) const noexcept; //!< check if species is involved in conservation
            bool linkedTo(const ConservationLaw &) const noexcept; //!< check if a species is shared
            void finalize();                                       //!< compute Q/Q2

            //! simple display
            friend std::ostream & operator<<(std::ostream &, const ConservationLaw &);

            //! compute excess, must be process if positive
            XReal excess(const readable<double> &C,
                         Adder                  &xadd,
                         const IndexLevel        level) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const CoreXList  Q;  //!< coefficients as extended real
            const XReal      Q2; //!< |Q|^2
            ConservationLaw *next; //!< for list
            ConservationLaw *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ConservationLaw);
        };

      
    }

}

#endif
