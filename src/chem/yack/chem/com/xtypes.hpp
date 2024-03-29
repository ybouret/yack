

#ifndef YACK_CHEM_XTYPES_INCLUDED
#define YACK_CHEM_XTYPES_INCLUDED 1

#include "yack/ipso/add.hpp"
#include "yack/ipso/mul.hpp"
#include "yack/data/dinky/core-list.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/proto-ostream.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"
#include "yack/math/triplet.hpp"

namespace yack {

    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! extended precision management
        //
        //______________________________________________________________________
        struct Extended
        {
            typedef extended<double>  Real;    //!< alias
            typedef ipso::add<double> Add;     //!< alias
            typedef arc_ptr<Add>      AddPtr;  //!< alias
            typedef ipso::mul<double> Mul;     //!< alias

            //! returm comparison as sign_type
            static inline sign_type   Comp(const Real &lhs, const Real &rhs) noexcept
            {
                return Real::scompare(lhs,rhs);
            }

            //! conversion from double to extended
            static inline Real   Send(const double x) { return ipso::inside<double>::send(x); }

            //! conversion from extended to double
            static inline double Recv(const Real &x) { return ipso::inside<double>::recv(x); }


            typedef core_list<Real>     CoreList; //!< alias
            typedef solo_list<Real>     SoloList; //!< alias
            typedef CoreList::node_type Node;     //!< alias

            typedef vector<Real,memory::dyadic> Vector; //!< alias
            typedef matrix<Real>                Matrix; //!< alias
            typedef math::triplet<Real>         Triplet; //!< alias
        };


        
        

    }

}

#endif

