//! \file

#ifndef YACK_CHEMICAL_EQUILIBRIA_INCLUDED
#define YACK_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/com/gathering.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // types for equilibria
        //
        //______________________________________________________________________
        typedef suffix_set<string,equilibrium::ptr> eqs_db; //!< database
        typedef eqs_db::knot_type                   enode;  //!< alias

        //______________________________________________________________________
        //
        //
        //! equilibrium = components + entity + constant(time)
        //
        //______________________________________________________________________
        class equilibria : public gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "equilibria"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria() noexcept; //!< setup emptys
            virtual ~equilibria() noexcept; //!< cleanup
            equilibria(const equilibria &); //!< hard cloning

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const eqs_db::tree_type & operator->() const noexcept; //!< drill-down

            size_t        next_indx() const noexcept;    //!< size+1
            equilibrium & operator()( equilibrium *eq ); //!< register
            YACK_PROTO_OSTREAM(equilibria);              //!< display

            //! display associated array
            template <typename ARR>
            std::ostream & operator()(std::ostream &os,
                                      ARR          &arr,
                                      const size_t  lvl,
                                      const char   *pfx = NULL) const
            {
                if(!pfx) pfx = "";
                os << '{' << std::endl;
                for(const enode *node=(*this)->head;node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    pad(os << "  " << pfx << eq,eq) << " = " << arr[ eq.indx[lvl] ] << std::endl;
                }
                os << '}';
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(equilibria);
            eqs_db edb;
        };

    }

}

#endif
