//! \file

#ifndef YACK_CHEM_EQ_INCLUDED
#define YACK_CHEM_EQ_INCLUDED 1

#include "yack/chem/eq/limits.hpp"
#include "yack/chem/actors.hpp"
#include "yack/chem/components.hpp"

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
        class equilibrium : public entity, public collection
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> pointer; //!< alias
            static const char                   clid[];  //!< chemical::equilibrium
            
            //! genus of equilibrium
            enum genus
            {
                is_unfinished, //!< no component
                has_both_ways, //!< reactant(s) and product(s)
                produces_only, //!< only product(s)
                consumes_only  //!< only reactant(s)
            };

            //! topology
            enum topology
            {
                topology_any,  //!< generic
                topology_0_1,  //!< . <=> A
                topology_1_0,  //!< A <=> .
                topology_0_2,  //!<   <=> 2A
                topology_1_1,  //!< A <=> B
                topology_0_11, //!< . <=> A+B
                topology_1_11  //!< A <=> B+C
            };

            //! topology to text
            static const char *topology_text(const topology) throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            virtual ~equilibrium() throw();

            //! setup
            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            entity(id),
            type(is_unfinished),
            topo(topology_any),
            reac(),
            prod(),
            nu_p(0),
            nu_r(0),
            d_nu(0),
            sexp(0),
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
            const char    *topo_text() const throw(); //!< topology_text(topo)

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
                    const species   &s = *c;
                    s(nu) = c.nu;
                }
            }


            //! set only active indices
            template <typename TARGET, typename SOURCE> inline
            void set(TARGET &target, SOURCE &source) const
            {
                for(const cnode *node=head();node;node=node->next)
                {
                    const size_t j  = *****node;
                    target[j] = source[j];
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
            const limits & find_private_limits(const readable<double> &C) const throw();

            //! find limits from primary species
            const limits & find_primary_limits(const readable<double> &C) const throw();

            //! solve single equilibria
            void   solve(const double K0, writable<double> &C, writable<double> &Ctry) const;


            //! find extent solving equilibrium, with optional vanishing species
            double extent(const double            K0,
                          const readable<double> &C,
                          writable<double>       &Ctmp) const;

            //! find generic extent
            double extent_(const double            K0,
                           const readable<double> &C,
                           writable<double>       &Ctmp) const;

            //! move valid C to valid Ctmp, return Ctmp
            writable<double> & move(writable<double> &Ctmp, const readable<double> &C, const double xi) const throw();

            //! maximum of reac/prod maxima
            double maximum(const readable<double> &C) const throw();

            //__________________________________________________________________
            //
            // checking
            //__________________________________________________________________
            bool   is_neutral() const throw(); //!< delta_r Z = 0
            void   validate()   const;         //!< not empty, neutral, minimal

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const genus    type; //!< qualification
            const topology topo; //!< specs
            const actors   reac; //!< list of reactant(s)
            const actors   prod; //!< list of product(s)
            const unit_t   nu_p; //!< sum nu_p
            const unit_t   nu_r; //!< sum nu_r
            const unit_t   d_nu; //!< nu_p-nu_r
            const double   sexp; //!< scaling exponent: 0 or 1/d_nu
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            const components    comp;
            mutable void       *wksp[ YACK_WORDS_FOR(limits) ];
            virtual double      getK(double) const = 0;
            void                update_genus() throw();
            void                build_family() throw();
            
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
