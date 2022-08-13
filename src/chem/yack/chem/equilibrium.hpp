//! \file

#ifndef YACK_CHEMICAL_EQ_INCLUDED
#define YACK_CHEMICAL_EQ_INCLUDED 1

#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {
        class equilibrium : public entity, public components
        {
        public:
            typedef ark_ptr<string,equilibrium> pointer;
            
            static double display_time; //!< default=0
            static size_t display_size; //!< default=12
            
            virtual ~equilibrium() throw();
           
            double K(double) const;

            friend std::ostream & operator<<(std::ostream &, const equilibrium&);

            
        protected:
            template <typename NAME> inline
            explicit equilibrium(const NAME &uid, const size_t idx) :
            entity(uid,idx), components()
            {
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double getK(double) const = 0;
        };
    }
    
}

#endif
