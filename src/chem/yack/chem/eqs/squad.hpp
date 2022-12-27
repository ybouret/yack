//! \file

#ifndef YACK_CHEMICAL_EQ_SQUAD_INCLUDED
#define YACK_CHEMICAL_EQ_SQUAD_INCLUDED 1

#include "yack/chem/eqs/team.hpp"

namespace yack
{
    namespace chemical
    {
        class eq_squad : public eq_team
        {
        public:
            explicit eq_squad() throw();
            virtual ~eq_squad() throw();
            eq_squad(const eq_squad &other);
            eq_squad *clone() const;

            eq_squad *next;
            eq_squad *prev;

            static int compare(const eq_squad *lhs, const eq_squad *rhs) throw();

        private:
            YACK_DISABLE_ASSIGN(eq_squad);
        };
        

        class eq_squads : public cxx_list_of<eq_squad>
        {
        public:
            explicit eq_squads() throw();
            virtual ~eq_squads() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_squads);
        };
    }

}

#endif

