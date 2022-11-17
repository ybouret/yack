//! \file

#ifndef YACK_CHEMICAL_EQ_INCLUDED
#define YACK_CHEMICAL_EQ_INCLUDED 1

#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {
        
        enum feature {
            join_only,
            both_ways,
            part_only
        };
        
        //______________________________________________________________________
        //
        //
        //! equilibrium = entity + components, with a K(time)
        //
        //______________________________________________________________________
        class equilibrium : public entity, public components
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> pointer; //!< alias
           
            
            static double display_time; //!< default=0
            static size_t display_size; //!< default=12


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            double  K(double) const;                                               //!< non-virtual, checkin getK()
            friend  std::ostream & operator<<(std::ostream &, const equilibrium&); //!< display
            void    vizlink(ios::ostream &) const;                                 //!< link to registered species
            feature kind() const; //!< get feature
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() throw();  //!< cleanup
            
        protected:
            //! set from name + index
            template <typename NAME> inline
            explicit equilibrium(const NAME &uid, const size_t idx) :
            entity(uid,idx), components()
            {
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double      getK(double) const = 0;
            virtual const char *attr()       const;
        };
    }
    
}

#endif
