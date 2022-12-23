
//! \file

#ifndef YACK_CHEMICAL_ACTORS_INCLUDED
#define YACK_CHEMICAL_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/xlimit.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/math/multiplier.hpp"
#include "yack/math/adder.hpp"
#include "yack/memory/workplace.hpp"
#include "yack/data/bare.hpp"

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


            const list_of<actor> * operator->() const throw(); //!< access to const crew
            bool attached_to(const actor  &) const throw();    //!< check link with actor
            bool attached_to(const actors &) const throw();    //!< check link with other actors
            //!

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


            //! fill array with coefficients at their position
            template <typename T> inline
            void fill( writable<T> &q ) const {
                const bare<T> _0; q.ld(*_0);
                for(const actor *a=crew.head;a;a=a->next)
                    q[***a] = a->nu;
            }

            bool must_balance(const readable<double>   &C,
                              double                   &extent,
                              meta_list<const species> &vanish) const;




            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned  molecularity; //!< molecularity = sum_i nu_i
            const int       algebraic_Z;  //!< algebraic_Z  = sum_i nu_i z_i

        protected:
            const cxx_list_of<actor>           crew; //!< actors
       
        private:
            mutable memory::workplace<xlimit>  wlim; //!< data for xlimit, standard
            //mutable memory::workplace<xlimit>  wbal; //!< data for xlimit, balancing
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
        };

    }

}

#endif
