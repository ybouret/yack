
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/component.hpp"
#include "yack/chem/actors.hpp"
#include "yack/chem/limits.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/data/list/meta.hpp"


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
        typedef meta_list<const species>                    alist; //!< alias
        typedef alist::node_type                            anode; //!< alias



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

            //! find private solving extent
            double solve1D(const double            K,
                           const readable<double> &C0,
                           writable<double>       &Cs) const;

            //! move to a given extent
            void move(writable<double> &C, const double xi) const throw();


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

            //! check valid concentrations
            bool are_valid(const readable<double> &C) const throw();

            //! helper to display
            std::ostream & display(std::ostream &os) const;

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
