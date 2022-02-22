//! \file

#ifndef YACK_CHEM_INTER_INCLUDED
#define YACK_CHEM_INTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"

namespace yack
{
    namespace chemical
    {

        class interaction : public const_equilibrium
        {
        public:
            typedef ark_ptr<string,interaction> pointer;
            virtual ~interaction() throw() {}

            template <typename NAME> inline
            explicit interaction(const int   self_coeff,
                                 const int   that_coeff,
                                 const NAME &that_name) :  const_equilibrium(that_name,1),
            next(0),
            prev(0),
            self(self_coeff),
            that(that_coeff)
            {
            }

            interaction *next;
            interaction *prev;
            const int    self;
            const int    that;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(interaction);
        };

        
        class connexions : public object, public counted,
        public authority<const equilibrium>
        {
        public:
            typedef arc_ptr<const connexions> pointer;
            
            explicit connexions(const equilibrium &) throw();
            virtual ~connexions() throw();
            cxx_list_of<interaction> add;
            cxx_list_of<interaction> sub;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(connexions);
        };


    }
}

#endif

