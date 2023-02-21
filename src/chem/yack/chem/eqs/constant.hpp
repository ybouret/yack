
//! \file

#ifndef YACK_CHEMICAL_EQS_CONST_INCLUDED
#define YACK_CHEMICAL_EQS_CONST_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class const_equilibrium : public equilibrium
        {
        public:
           
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~const_equilibrium() noexcept; //!< cleanup

            //! setup with name, index and K value
            template <typename NAME> inline
            explicit const_equilibrium(const NAME &uid,
                                       const size_t idx,
                                       const double val) :
            equilibrium(uid,idx), K_(val)
            {
            }
            
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
            const double K_;
            virtual double getK(double) const;
        };
    }
    
}
#endif

