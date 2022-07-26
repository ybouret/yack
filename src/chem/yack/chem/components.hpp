//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/component.hpp"
#include "yack/chem/actors.hpp"
#include "yack/chem/limits.hpp"
#include "yack/associative/suffix/set.hpp"

#include <iomanip>

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef suffix_set<string,const component::pointer> components_set; //!< alias
        typedef components_set::knot_type                   cnode;          //!< alias
      

        //! extent state w.r.t its equilibria
        enum extent_state
        {
            significant_extent, //!< no underflow
            degenerated_extent  //!< at least one underflow
        };

        //! components state w.r.t to its actors
        enum components_state
        {
            running_components,
            blocked_components
        };

        //! outcome of 1D brewing
        class outcome
        {
        public:
            const extent_state     has; //!< qualified extent
            const components_state are; //!< components state
            const double           xi0; //!< solving extent

            //! setup
            outcome(const extent_state xs, const components_state cs, const double xx) throw() :
            has(xs),
            are(cs),
            xi0(xx)
            {
            }

            //! cleanup
            ~outcome() throw() {}

            //! copy
            outcome(const outcome &other) throw() :
            has(other.has),
            are(other.are),
            xi0(other.xi0)
            {

            }

            //! display info
            friend std::ostream & operator<<(std::ostream &os, const outcome res)
            {
                switch(res.are)
                {
                    case running_components: os << "[running]"; break;
                    case blocked_components: os << "[blocked]"; break;
                }

                switch(res.has)
                {
                    case significant_extent: os << "(+)"; break;
                    case degenerated_extent: os << "(0)"; break;
                }

                os << "@" << std::setw(15) << res.xi0;

                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(outcome);
        };



        //______________________________________________________________________
        //
        //
        //! set of unique components
        //
        //______________________________________________________________________
        class components
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit components() throw(); //!< setup
            virtual ~components() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // I/O methods
            //__________________________________________________________________
            const cnode *head() const throw(); //!< first component
            size_t       size() const throw(); //!< number of components

            //! append a new component, false if species was already registered
            bool operator()(const species &sp,
                            const int      nu);


            //! access component by index
            const component & operator[](const size_t k) const throw();

            //! fill a row of matrix
            template <typename T> inline
            void fill(writable<T> &nu) const
            {
                for(const cnode *node=head();node;node=node->next)
                {
                    const component &c = ***node;
                    const species   &s = *c;
                    nu[*s] = c.coef;
                }
            }

            //! transfer only used species
            template <typename T, typename U> inline
            void transfer(writable<T> &target, const readable<U> &source) const
            {
                for(const cnode *node=head();node;node=node->next)
                {
                    const size_t j = *****node;
                    target[j] = static_cast<T>( source[j] );
                }
            }

            //! test (by index) if components are detached (a.k.a no common species)
            bool detached(const components &other) const throw();

            //! test (by index) if components are attached (a.k.a share common species)
            bool attached(const components &other) const throw();

            //__________________________________________________________________
            //
            // computation methods
            //__________________________________________________________________

            //! reac.mass_action(K,C) - prod.mass_action(1,C)
            double         mass_action(const double K, const readable<double> &C) const throw();

            //! reac.mass_action(K,C,-xi) - prod.mass_action(1,C,xi)
            double         mass_action(const double K, const readable<double> &C, const double xi) const throw();

            //! -(K*reac.slope(K,C,-xi) + prod.slope(C,xi))
            double         slope(const double K, const readable<double> &C, const double xi) const throw();

            //! gradient
            void           drvs_action(writable<double> &psi, const double K, const readable<double> &C, writable<double> &arr) const throw();

            //! gradient + mass_acttion
            double         grad_action(writable<double> &psi, const double K, const readable<double> &C, writable<double> &arr) const throw();

            //! hessian
            void hessian(matrix<double> &H, const double K, const readable<double> &C, writable<double> &arr) const throw();

            //! find private solving extent
            double solve1D(const double            K,
                           const readable<double> &C0,
                           writable<double>       &Cs) const;

            //! find private solving outcome
            outcome brew1D(const double            K,
                           const readable<double> &C0,
                           writable<double>       &Cs,
                           size_t                *calls=0) const;

            //! move to a given extent
            void move(writable<double> &C, const double xi) const throw();

            //! detect degenerated: at lease one is too small
            bool found_degenerated(const double xi, const readable<double> &C) const throw();

            //! detect significant: none is too small
            bool found_significant(const double xi, const readable<double> &C) const throw();

            //! qualify extent value w.r.t involved concentrations
            extent_state qualify_extent(const double xi, const readable<double> &C) const throw();

            //! find private limits
            const limits & private_limits(const readable<double> &C, const size_t w) const throw();

            //! find primary limits
            const limits & primary_limits(const readable<double> &C, const size_t w) const throw();


            //! max_of(reac.maximum,prod.maximum)
            double maximum(const readable<double> &C) const throw();

            //! quotient
            double Q(const readable<double> &C) const throw();

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! check neutrality
            bool is_neutral() const throw();

            //! check minimal coefficients
            bool is_minimal() const throw();

            //! full check
            void must_comply(const string &name) const;

            //! check valid concentrations
            bool are_valid(const readable<double> &C) const throw();

            //! helper to display
            std::ostream & display(std::ostream &os) const;

            //! display signature
            std::ostream & display_signature(std::ostream &os) const;


            //! check other components match
            bool other_are_unchanged(const readable<double> &lhs, const readable<double> &rhs) const throw();

            //! check species index is in the components
            bool found_species_index(const size_t i) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors      reac; //!< list of reactants
            const actors      prod; //!< list of products
            const int         d_nu; //!< Delta_r nu
            const double      sexp; //!< scaling exponent: Cs approx pow(K,1.0/d_nu)
            const int         nu_p; //!< Delta_r nu_p >= 0
            const int         nu_r; //!< Delta_r nu_r >=0
            const string      topo; //!< named topology

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
            components_set db;
            mutable void *wksp[ YACK_WORDS_FOR(limits) ];

            void   update() throw();
            void   mktopo();
            double deduce(const readable<double> &C0, const readable<double> &Cs) const throw();
        };

    }

}

#endif
