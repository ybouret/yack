
//! \file

#ifndef YACK_CHEMICAL_COMPONENTS_INCLUDED
#define YACK_CHEMICAL_COMPONENTS_INCLUDED 1

#include "yack/chem/eqs/component.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/data/dinky/coop-repo.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/cameo/add.hpp"

namespace yack
{
    
    namespace chemical
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
            const components_db::tree_type & operator->() const noexcept; //!< get internal tree
            void   operator()(const int nu, const species &sp);           //!< declare a new components
            string to_string()  const;                                    //!< stringify
            void   report_to(addrbook &) const;                           //!< append species
            bool   is_neutral() const noexcept;                           //!< check neutrality
            bool   is_minimal() const noexcept;                           //!< check co-primality
            YACK_PROTO_OSTREAM(components);                               //!< display

            //! compute mass action
            double mass_action(const index_level       I,
                               const readable<double> &C,
                               const double            K,
                               cameo::mul<double>     &xmul) const;

            //! compute mass action with extent
            double mass_action(const index_level        I,
                               const readable<double>  &C,
                               const double             xi,
                               const double             K,
                               cameo::mul<double>      &xmul) const;

            //! move C with hard cut
            void move(writable<double> &C, const double xi) const noexcept;

            //! move C
            void mov_(writable<double> &C, const double xi) const noexcept;

            //! gradient
            void grad( const index_level       I,
                      writable<double>       &psi,
                      const readable<double> &C,
                      const double            K,
                      cameo::mul<double>     &xmul) const;

            //! compute slope at given concentraion
            double slope( const index_level       I,
                         const readable<double> &C,
                         const double            K,
                         cameo::mul<double>     &xmul,
                         cameo::add<double>     &xadd) const;

            //! evaluate quotient
            double quotient(const index_level       I,
                            const readable<double> &C,
                            const double            K,
                            cameo::mul<double>     &xmul) const;

            //! check if species is used
            bool is_connected_to(const species &)    const noexcept;

            //! check if a species is shaed
            bool is_connected_to(const components &) const noexcept;

            
            //! transfer coefficients
            template <typename ARR> inline
            void fill( const unsigned level, ARR &nu) const
            {
                nu.ld(0);
                for(const cnode *node=(*this)->head;node;node=node->next)
                {
                    const component &c = ***node;
                    const species   &s = *c;
                    nu[s.indx[level]] = c.nu;
                }
            }

            //! display values of top-level array
            template <typename ARR> inline
            std::ostream & display_compact(std::ostream &os, ARR &arr) const
            {
                os << '{';
                for(const cnode *node=(*this)->head;node;node=node->next)
                {
                    const component &c = ***node;
                    const species   &s = *c;
                    os << ' ' << s << '=' << arr[ s.indx[top_level] ];
                }
                os << ' ' << '}';
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors reac; //!< list of reactants
            const actors prod; //!< list of products
            const int    d_nu; //!< D_r Nu
            const double idnu; //!< 1.0/|d_nu|

        private:
            YACK_DISABLE_ASSIGN(components);
            components_db cdb;
            
        };

    }

}

#endif
