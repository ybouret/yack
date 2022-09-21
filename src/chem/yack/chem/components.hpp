
//! \file

#ifndef YACK_CHEMICAL_COMPONENTS_INCLUDED
#define YACK_CHEMICAL_COMPONENTS_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/chem/component.hpp"
#include "yack/chem/xlimits.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/data/bare.hpp"

namespace yack
{
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
            // methods
            //__________________________________________________________________
            
            //! insert a new components
            void operator()(const species &sp,
                            const int      nu);
            
            //! get first component
            const cnode   *head() const throw();
            
            //! number of components
            size_t         size() const throw();
            
            //! limits from all the components
            const xlimits &genuine_limits(const readable<double> &C, const size_t w) const throw();

            //! limits from all the primary components
            const xlimits &primary_limits(const readable<double> &C, const size_t w) const throw();


            //! display reac <=> prod
            friend std::ostream & operator<<(std::ostream &, const components &);

            //! compute mass action for a given constant
            double mass_action(const double            K,
                               const readable<double> &C,
                               rmulops                &) const;


            //! compute mass action for a given fraction of Cini->Cend
            double mass_action(const double            K,
                               const readable<double> &Cini,
                               const readable<double> &Cend,
                               const double            u,
                               writable<double>       &Ctry,
                               rmulops                &) const;

            
            //! compute quotient reaction
            double quotient(const double            K,
                            const readable<double> &C,
                            rmulops                &) const;

            //! compute mass action for a given constant and xi
            double mass_action(const double            K,
                               const readable<double> &C,
                               const double            xi,
                               rmulops                &xmul) const;

            //! compute gradient of mass action
            void grad_action(writable<double>       &psi,
                             const double            K,
                             const readable<double> &C,
                             rmulops                &xmul) const;

            //! compute gradient and derivative of mass action
            double diff_action(writable<double>       &psi,
                               const double            K,
                               const readable<double> &C,
                               rmulops                &xmul,
                               raddops                &xadd) const;

            //!  compute gradient and derivative of mass action for a given fraction of Cini->Cend
            double diff_action(writable<double>       &psi,
                               const double            K,
                               const readable<double> &Cini,
                               const readable<double> &Cend,
                               const double            u,
                               writable<double>       &Ctry,
                               rmulops                &xmul,
                               raddops                &xadd) const;



            
            //! move C with computed extent
            void move(writable<double> &C, const double xi) const throw();

            //! fill topology
            template <typename T> inline void fill(writable<T> &nu) const
            {
                const bare<T> _0;
                nu.ld(*_0);
                for(const cnode *node=head();node;node=node->next)
                {
                    const component &comp = ***node;
                    nu[**comp] = comp.nu;
                }
            }

            //! transfer only concerned components
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source) const
            {
                for(const cnode *node=head();node;node=node->next)
                {
                    const size_t j = *****node;
                    target[j] = source[j];
                }
            }

            //! parse string, use library for species
            void operator()(library &, const string &);

            //! parse string, use library for species
            void operator()(library &, const char   *);

            //! check DrZ = 0
            bool neutral() const throw();

            //! check minimal
            bool minimal() const throw();

            //! check is running or blocked
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
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors reac; //!< reactants, nu<0
            const actors prod; //!< products,  nu>0
            const int    d_nu; //!< prod.molecularity - reac.molecularity
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);

            //__________________________________________________________________
            //
            // private members
            //__________________________________________________________________
            components_set                     cdb; //!< components database
            mutable memory::workplace<xlimits> xlm; //!< workplace for xlimits

        };
        
    }
    
}

#endif
