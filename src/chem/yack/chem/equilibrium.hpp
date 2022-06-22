//! \file

#ifndef YACK_CHEM_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/components.hpp"
#include "yack/type/v2d.hpp"

namespace yack
{
    namespace chemical
    {
        class library;


        //______________________________________________________________________
        //
        //
        //! named components and constant
        //
        //______________________________________________________________________
        class equilibrium : public entity, public components
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static double                       display_time;  //!< for standalone print
            static size_t                       display_width; //!< for standalone print
            typedef ark_ptr<string,equilibrium> pointer;       //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            double K(const double t) const;              //!< checked getK(t)
            void   parse_with(library &,const string &); //!< parse components
            
            //! standalone output
            friend std::ostream & operator<<(std::ostream &, const equilibrium &);


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() throw(); //!< cleanup

        protected:
            //! setup
            template <typename ID> inline
            equilibrium(const ID &uuid, const size_t ipos) :
            entity(uuid,ipos),
            components()
            {
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double getK(const double) const = 0;
            std::ostream & print(std::ostream &) const;
        };

      
      

    }

}

#endif
