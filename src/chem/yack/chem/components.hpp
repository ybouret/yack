
//! \file

#ifndef YACK_CHEMICAL_COMPONENTS_INCLUDED
#define YACK_CHEMICAL_COMPONENTS_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/chem/component.hpp"
#include "yack/chem/xlimits.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/associative/addrbook.hpp"


namespace yack
{
    class xmlog;
    
    namespace chemical
    {



        //______________________________________________________________________
        //
        // global types
        //______________________________________________________________________
        typedef suffix_set<string,const component::pointer> components_set; //!< alias
        typedef components_set::knot_type                   cnode;          //!< alias
        class library;

        //______________________________________________________________________
        //
        //
        //! qualifying extents
        //
        //______________________________________________________________________
        struct extent
        {
            //__________________________________________________________________
            //
            //! grade values
            //__________________________________________________________________
            enum grade
            {
                is_significant, //!< changes balance
                is_degenerated  //!< doesn't change balance
            };
        };

        

        //______________________________________________________________________
        //
        //
        //! components=set of unique components
        //
        //______________________________________________________________________
        class components
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< "components"

            //! current state
            enum state
            {
                are_running, //!< may (already) be balanced
                are_blocked  //!< blocked
            };

            //! get textual value
            static const char *state_text(const state s) throw();


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit components() throw(); //!< setup empty
            virtual ~components() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods to build
            //__________________________________________________________________

            //! insert a new components
            void operator()(const species &sp,
                            const int      nu);

            //! parse string, use library for species
            void operator()(library &, const string &);

            //! parse string, use library for species
            void operator()(library &, const char   *);

            //__________________________________________________________________
            //
            // methods to access
            //__________________________________________________________________

            //! get first component
            const cnode   *head() const throw();
            
            //! number of components
            size_t         size() const throw();

            //! max index of components
            size_t         span() const throw();

            //! query is species is used
            const component *query(const species &s) const throw();

            //__________________________________________________________________
            //
            // comptuing limits to solve 1D
            //__________________________________________________________________

            //! limits from all the components
            const xlimits &genuine_limits(xlimits_io             &xio,
                                          const readable<double> &C,
                                          const size_t            w) const throw();

            

            //! display reac <=> prod
            YACK_PROTO_OSTREAM(components);

            //! make a string
            string to_string() const;

            //__________________________________________________________________
            //
            // computing mass action
            //__________________________________________________________________

            //! compute mass action for a given constant
            double mass_action(const double            K,
                               const readable<double> &C,
                               rmulops                &xmul) const;


            //! compute mass action for a given constant, with scaling
            double mass_action(const double            K,
                               const readable<double> &C,
                               rmulops                &xmul,
                               const double            scal) const;



            //! compute mass action for a given constant and xi
            double mass_action(const double            K,
                               const readable<double> &C,
                               const double            xi,
                               rmulops                &xmul) const;


            //! compute mass action for a given fraction of Cini->Cend
            double mass_action(const double            K,
                               const readable<double> &Cini,
                               const readable<double> &Cend,
                               const double            u,
                               writable<double>       &Ctry,
                               rmulops                &xmul) const;


            //! compute mass action for a given fraction of Cini->Cend, with scaling
            double mass_action(const double            K,
                               const readable<double> &Cini,
                               const readable<double> &Cend,
                               const double            u,
                               writable<double>       &Ctry,
                               rmulops                &xmul,
                               const double            scal) const;


            
            //! compute quotient reaction
            double quotient(const double            K,
                            const readable<double> &C,
                            rmulops                &) const;

            //! compute gradient of mass action, return |psi|_max
            greatest grad_action(writable<double>       &psi,
                                 const double            K,
                                 const readable<double> &C,
                                 rmulops                &xmul) const;


            //__________________________________________________________________
            //
            // methods to handle 
            //__________________________________________________________________


            //! move C with computed extent
            void move(writable<double> &C, const double xi) const throw();

            //! return true is new species were inserted
            bool update(addrbook &tribe) const;

            //! fill topology
            template <typename T> inline void fill(writable<T> &nu) const
            {
                const bare<T> _0;
                nu.ld(*_0);
                for(const cnode *node=head();node;node=node->next) {
                    const component &comp = ***node;
                    nu[**comp] = comp.nu;
                }
            }

            //! test proportionality of coefficients
            static bool are_similar(const components &lhs, const components &rhs);


            //! transfer only involved components
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source) const
            {
                for(const cnode *node=head();node;node=node->next) {
                    const size_t j = *****node;
                    target[j] = source[j];
                }
            }


            //! check Delta_r Z = 0
            bool neutral() const throw();

            //! check minimal
            bool minimal() const throw();

            //! check is running or blocked by are_primarily_blocked
            state state_at(const readable<double> &C) const throw();

            //! deduce extent from difference of concentrations
            double       estimate_extent(const readable<double> &Cini, const readable<double> &Cend, raddops & ) const;

            //! evaluate grade of extent
            extent::grade qualify_extent(const double            K,
                                         const readable<double> &Cend,
                                         const readable<double> &Cini,
                                         rmulops                &xmul) const ;

            //! check if one species in common
            bool attached_to(const components &other) const throw();

            //! check is no species in common
            bool detached_of(const components &other) const throw();

            //! check is use species
            bool uses(const species &) const throw();


            //! display list of associated concentrations
            std::ostream & display_compact(std::ostream &os, const readable<double> &C) const;

            //! display list of associated conserved species
            std::ostream & display_compact(std::ostream &os, const readable<double> &C, const readable<bool> &flag) const;
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors  reac; //!< reactants, nu<0
            const actors  prod; //!< products,  nu>0
            const int     d_nu; //!< prod.molecularity - reac.molecularity
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);

            //__________________________________________________________________
            //
            // private members
            //__________________________________________________________________
            components_set                     cdb; //!< components database
            
        public:
            
            //! helper for aftermath
            struct mass_action_call {
                double                  K_eq; //!< local constant
                const components       &self; //!< local components
                const readable<double> &conc; //!< phase space
                rmulops                &xmul; //!< operations
                double operator()(double xi); //!< mass_action at conc+nu*xi*
                
            };
        };
        
    }
    
}

#endif
