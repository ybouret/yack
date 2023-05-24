
//! \file

#ifndef YACK_CHEM_EQUILIBRIA_INCLUDED
#define YACK_CHEM_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Equilibrium::Pointer> EqsDB;
        typedef EqsDB::knot_type                        eNode;
        typedef list_of<eNode>                          eList;

        class Equilibria : public Gathering
        {
        public:
            static const char CLID[];

            explicit Equilibria() noexcept;
            virtual ~Equilibria() noexcept;

            const eList * operator->() const noexcept;

            template <typename EQID> inline
            EQID & operator()(EQID *eq)
            {
                assert(NULL!=eq);
                add(eq);
                return *eq;
            }

            //! fill TopLevel Nu
            template <typename T> inline
            void fill( matrix<T> &Nu ) const
            {
                for(const eNode *node=(*this)->head;node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    eq.fill(Nu[eq.indx[TopLevel]],TopLevel);
                }
            }

            friend std::ostream & operator<<(std::ostream &, const Equilibria &);

            void finalize() noexcept;

            Gathering reac;
            Gathering prod;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equilibria);
            EqsDB edb;

            void add(Equilibrium *eq);
        };
    }

}

#endif

