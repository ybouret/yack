
//! \file

#ifndef YACK_CHEMICAL_ACTORS_INCLUDED
#define YACK_CHEMICAL_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/xlimit.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/math/multiplier.hpp"
#include "yack/math/adder.hpp"
#include "yack/memory/workplace.hpp"

namespace yack
{
    namespace chemical
    {


        //______________________________________________________________________
        //
        //
        // global definitions
        //
        //______________________________________________________________________
        typedef math::multiplier<double> rmulops; //!< alias
        typedef math::adder<double>      raddops; //!< alias

        //______________________________________________________________________
        //
        //
        //! list of actores
        //
        //______________________________________________________________________
        class actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~actors() throw(); //!< cleanup
            explicit actors() throw(); //!< setup empty

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            
            //! append a new actor(sp,nu>0);
            void operator()(const species &sp,
                            const unsigned nu);

            //! emergency remove last
            void remove_last() throw();

            //! mass action with rmulops
            double mass_action(const readable<double> &C,
                               rmulops                &ops) const;

            
            //! mass action with rmulops, C+nu*xi >=0 
            double mass_action(const readable<double> &C,
                               const double            xi,
                               rmulops                &ops) const;

            //! C -> max_of(C+nu*xi,0) (sign of xi must be adapted)
            void  move(writable<double> &C, const double xi) const throw();

            //! C -> C+nu*xi, raw value, sign of xi must be adapted
            void  mov_(writable<double> &C, const double xi) const throw();



            const xlimit     *genuine_limit(const readable<double> &C) const throw(); //!< test all actors
            const xlimit     *primary_limit(const readable<double> &C) const throw(); //!< test all primary actors
            const xlimit     *primarily_bad(const readable<double> &C) const throw(); //!< test for invalid actors
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const actors &);

            //! check that any of the acting concentration is negative
            bool are_genuinely_blocked_by(const readable<double> &C) const throw();

            //! check that any of the primary acting concentration is negative
            bool are_primarily_blocked_by(const readable<double> &C) const throw();

            //! compute derivatives of mass action w.r.t C
            void grad_action(writable<double>       &psi,
                             const double            factor,
                             const readable<double> &C,
                             rmulops                &xmul) const;
            
            //! display list of compact associated concentrations
            void display_compact(std::ostream &, const readable<double> &C) const;

            //! access operator to use list
            const list_of<const actor> * operator->() const throw()
            {
                const list_of<actor> &self = crew;
                return & coerce_to< list_of<const actor> >(self);
            }

            //! check
            inline bool find_species_of(const actor &lhs) const throw()
            {
                const size_t j = **lhs;
                for(const actor *rhs=crew.head;rhs;rhs=rhs->next)
                {
                    if( j == ***rhs) return true;
                }
                return false;
            }

            //! check
            inline bool find_shared_species(const actors &other) const throw()
            {
                for(const actor *lhs=other->head;lhs;lhs=lhs->next)
                {
                    if( find_species_of(*lhs) ) return true;
                }
                return false;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned  molecularity; //!< molecularity = sum_i nu_i
            const int       algebraic_Z;  //!< algebraic_Z  = sum_i nu_i z_i
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            cxx_list_of<actor>                 crew; //!< actors
            mutable memory::workplace<xlimit>  wlim; //!< data for xlimit, standard
            mutable memory::workplace<xlimit>  wbal; //!< data for xlimit, balancing
        };

    }

}

#endif
