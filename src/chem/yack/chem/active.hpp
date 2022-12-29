
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/math/adder.hpp"
#include "yack/chem/sp/list.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        // types for list of active species
        //
        //______________________________________________________________________
        typedef sp_list::node_type         anode; //!< alias for node of active species

        //______________________________________________________________________
        //
        //
        //! list of active species
        //
        //______________________________________________________________________
        class active_list : public sp_list
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const active_list> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit active_list() throw();      //!< setup empty
            virtual ~active_list() throw();      //!< cleanup
            explicit active_list(const snode *); //!< build from species list

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


#if 0
            //! target = max_of(0,LHS+RHS)
            template <typename TARGET, typename LHS, typename RHS> inline
            void add_safe(TARGET &target, LHS &lhs, RHS &rhs) const
            {
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t                j = ***node;
                    typename TARGET::const_type t = lhs[j]+rhs[j];
                    target[j] = (t<=0) ? 0 : t;
                }
            }

            //! restricted dot product
            template <typename LHS, typename RHS> inline
            double dot(LHS &lhs, RHS &rhs, math::adder<double> &xadd) const
            {
                xadd.free();
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    const double l = static_cast<double>(lhs[j]);
                    const double r = static_cast<double>(rhs[j]);
                    xadd << l*r;
                }
                return xadd.get();
            }

            //! restricted |lhs|^2
            template <typename LHS> inline
            double norm2(LHS &lhs, math::adder<double> &xadd) const
            {
                xadd.free();
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    const double l = static_cast<double>(lhs[j]);
                    xadd << l*l;
                }
                return xadd.get();
            }

            //! restricted |lhs-rhs|^2
            template <typename LHS, typename RHS> inline
            double norm2(LHS &lhs, RHS &rhs, math::adder<double> &xadd) const
            {
                xadd.free();
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    const double l = static_cast<double>(lhs[j]);
                    const double r = static_cast<double>(rhs[j]);
                    const double d = l-r;
                    xadd << d*d;
                }
                return xadd.get();
            }
#endif
            




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };
        
    }

}

#endif
