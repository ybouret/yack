

//! \file

#ifndef YACK_CHEM_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/components.hpp"


namespace yack
{
    namespace chemical
    {
        class library;

        class equilibrium : public entity, public components
        {
        public:
            static double display_time;  //!< for standalone print
            static size_t display_width; //!< for standalone print

            typedef ark_ptr<string,equilibrium> pointer;
            virtual ~equilibrium() throw();

            double K(const double t) const;
            void   parse_with(library &,const string &);

            //! standalone output
            friend std::ostream & operator<<(std::ostream &, const equilibrium &);



        protected:
            template <typename ID> inline
            equilibrium(const ID &uuid, const size_t ipos) :
            entity(uuid,ipos),
            components()
            {
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            std::ostream & print(std::ostream &) const;
            virtual double getK(const double) const = 0;
        };

        class const_equilibrium : public equilibrium
        {
        public:
            template <typename ID> inline
            const_equilibrium(const ID    &uuid,
                              const size_t ipos,
                              const double kval) :
            equilibrium(uuid,ipos),
            value(kval)
            {
            }

            virtual ~const_equilibrium() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
            const double value;
            virtual double getK(const double) const;
        };

    }

}

#endif

