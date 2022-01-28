

#ifndef YACK_CHEM_EQ_INCLUDED
#define YACK_CHEM_EQ_INCLUDED 1

#include "yack/chem/actors.hpp"
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
            double         K(double) const;      //!< check valid getK(t)
            virtual size_t size() const throw(); //!< number of registered components
            const cnode   *head() const throw(); //!< fist component

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void add(const species &sp, const unit_t nu); //!< add a registers species
            void load(const string &expr, library &lib);  //!< load from string
            void load(const char   *expr, library &lib);  //!< load from string

            //! display with name witdh alignment at time t
            void display(std::ostream &os, const size_t w, const double t) const;

            //! display w=0, t=0
            friend std::ostream & operator<<(std::ostream &, const equilibrium &);

            //! K * prod - reac
            double mass_action(const double K0, const readable<double> &C) const;


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
            virtual double      getK(double) const = 0;

        };

        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class const_equilibrium : public equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~const_equilibrium() throw();

            //! setup
            template <typename ID> inline
            explicit const_equilibrium(const ID &id, const double K_) :
            equilibrium(id), value(K_)
            {}



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
            const double value;
            virtual double getK(double) const;

        };


    }

}

#endif
