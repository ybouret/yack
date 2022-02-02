//! \file

#ifndef YACK_CHEM_EQ_INCLUDED
#define YACK_CHEM_EQ_INCLUDED 1

#include "yack/chem/eq/limits.hpp"
#include "yack/chem/actors.hpp"
#include "yack/chem/components.hpp"
#include "yack/type/gateway.hpp"

namespace yack
{

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! alias to loop over components
        //
        //______________________________________________________________________
        typedef components::knot_type cnode;

        //______________________________________________________________________
        //
        //
        //! named equilibrium
        //
        //______________________________________________________________________
        class equilibrium : public large_object, public counted,
        public gateway<const components>,
        public collection
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> pointer; //!< alias

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
            indx(0),
            nu_p(0),
            nu_r(0),
            d_nu(0),
            comp(),
            wksp()
            {
            }

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            double         K(double) const;      //!< check valid getK(t)
            virtual size_t size() const throw(); //!< number of registered components
            const cnode   *head() const throw(); //!< fist component
            const string  &key()  const throw(); //!< name for equilibria
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void add(const species &sp, const unit_t nu); //!< add a registers species
            void load(const string &expr, library &lib);  //!< load from string
            void load(const char   *expr, library &lib);  //!< load from string

            //! fill row of topology
            template <typename T> inline
            void fill(writable<T> &nu) const
            {
                for(const cnode *node=head();node;node=node->next)
                {
                    const component &c = ***node;
                    c.sp(nu) = c.nu;
                }
            }



            //! display with name witdh alignment at time t
            void display(std::ostream &os, const size_t w, const double t) const;

            //! display w=0, t=0
            friend std::ostream & operator<<(std::ostream &, const equilibrium &);

            //! K * prod - reac
            double mass_action(const double K0, const readable<double> &C) const throw();

            //! derivative of mass action, return mass action
            double drvs_action(writable<double> &psi, const double K0, const readable<double> &C) const throw();

            //! find full limits for single equilibrium
            const limits & find_limits(const readable<double> &C) const throw();

            //! move to a null mass action
            void solve(const double K0, writable<double> &C, writable<double> &Ctry) const throw();

            //__________________________________________________________________
            //
            // checking
            //__________________________________________________________________
            bool is_neutral() const throw(); //!< delta_r Z = 0
            void validate()  const;          //!< not empty, neutral

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            const actors reac; //!< list of reactant(s)
            const actors prod; //!< list of product(s)
            const size_t indx; //!< in equilibria
            const unit_t nu_p; //!< sum nu_p
            const unit_t nu_r; //!< sum nu_r
            const unit_t d_nu; //!< nu_p-nu_r
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            const components comp;
            mutable void    *wksp[ YACK_WORDS_FOR(limits) ];
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
