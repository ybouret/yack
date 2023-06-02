//! \file

#ifndef YACK_CHEM_EQUILIBRIA_INCLUDED
#define YACK_CHEM_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Equilibrium::Pointer> EqsDB; //!< alias
        typedef EqsDB::knot_type                        eNode; //!< alias
        typedef list_of<eNode>                          eList; //!< alias

        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class Equilibria : public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Equilibria"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equilibria() noexcept; //!< setup
            virtual ~Equilibria() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const eList * operator->() const noexcept; //!< access

            //! register a new equilibrium, update Gathering
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


            YACK_PROTO_OSTREAM(Equilibria); //!< display
            void finalize() noexcept;       //!< setup reac/proc gathering for clean display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Gathering reac; //!< for all reactants
            Gathering prod; //!< for all products

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equilibria);
            EqsDB edb;

            void add(Equilibrium *eq);
        };
    }

}

#endif

