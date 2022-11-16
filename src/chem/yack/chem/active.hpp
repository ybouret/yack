
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/data/list/meta.hpp"
#include "yack/math/adder.hpp"

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
        typedef meta_list<const species> alist; //!< alias for list of active species
        typedef alist::node_type         anode; //!< alias for node of active species

        //______________________________________________________________________
        //
        //
        //! list of active species
        //
        //______________________________________________________________________
        class active_list : public alist
        {
        public:
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

            //! transfer only active part
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source) const
            {
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = source[j];
                }
            }

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

            




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };
        
    }

}

#endif
