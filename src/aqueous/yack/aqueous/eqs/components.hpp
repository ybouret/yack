
//! \file

#ifndef YACK_AQUEOUS_COMPONENTS_INCLUDED
#define YACK_AQUEOUS_COMPONENTS_INCLUDED 1

#include "yack/aqueous/eqs/component.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/data/dinky/coop-repo.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        // types for components
        //
        //______________________________________________________________________
        typedef suffix_set<string,component::ptr> components_db; //!< databse of components
        typedef components_db::knot_type          cnode;         //!< alias
  

        //______________________________________________________________________
        //
        //
        //! components are a database of components
        //
        //______________________________________________________________________
        class components : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "components"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit components() noexcept; //!< setup empty
            virtual ~components() noexcept; //!< cleanup
            components(const components &); //!< hard copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const components_db::tree_type & operator->() const noexcept;      //!< get internal tree

            void operator()(const int nu, const species &sp); //!< declare a new components
            YACK_PROTO_OSTREAM(components);                   //!< display
            string to_string() const;                         //!< stringify

            //! transfer coefficients
            template <typename ARR> inline
            void fill( ARR &nu, const unsigned level) const
            {
                for(const cnode *node=(*this)->head;node;node=node->next)
                {
                    const component &c = ***node;
                    const species   &s = *c;
                    nu[s.indx[level]] = c.nu;
                }
            }

            //! compute mass action
            double mass_action(const readable<double> &C,
                               const double            K,
                               cameo::mul<double>     &xmul) const;

            //! compute mass action with extent
            double mass_action(const readable<double> &C,
                               const double            xi,
                               const double            K,
                               cameo::mul<double>     &xmul) const;

            bool is_neutral() const noexcept; //!< check neutrality
            bool is_minimal() const noexcept; //!< check co-primality

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors reac; //!< list of reactants
            const actors prod; //!< list of products
            
        private:
            YACK_DISABLE_ASSIGN(components);
            components_db cdb;
            
        };

    }

}

#endif
