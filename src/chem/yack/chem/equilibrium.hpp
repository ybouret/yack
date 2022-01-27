

#ifndef YACK_CHEM_EQ_INCLUDED
#define YACK_CHEM_EQ_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/components.hpp"
#include "yack/type/gateway.hpp"

namespace yack
{

    namespace chemical
    {

        typedef components::knot_type cnode; //!< alias

        //______________________________________________________________________
        //
        //
        //! named equilibrium
        //
        //______________________________________________________________________
        class equilibrium : public object, public counted,
        public gateway<const components>,
        public collection
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            virtual ~equilibrium() throw();

            //! setup
            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            name(id),
            reac(),
            prod(),
            comp()
            {
            }

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t size() const throw(); //!< number of registered components
            const cnode   *head() const throw(); //!< fist component

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void add(const species &sp, const unit_t nu); //!< add a registers species
            void load(const string &expr, library &lib);  //!< load from string
            void load(const char   *expr, library &lib);  //!< load from string

            void display(std::ostream &os, const size_t w, const double t) const;

            friend std::ostream & operator<<(std::ostream &, const equilibrium &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            const actors reac; //!< list of reactant(s)
            const actors prod; //!< list of product(s)

        private:
            const components comp;
            virtual const_type &bulk() const throw();
        };

    }

}

#endif
