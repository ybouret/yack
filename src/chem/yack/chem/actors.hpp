
//! \file

#ifndef YACK_CHEMICAL_ACTORS_INCLUDED
#define YACK_CHEMICAL_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/xlimit.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/math/multiplier.hpp"
#include "yack/memory/workplace.hpp"

namespace yack
{
    namespace chemical
    {
        
        typedef math::multiplier<double> rmulops; //!< alias

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

            //! test all actors
            const xlimit     *genuine_limit(const readable<double> &C) const throw();
          
            //const xlimit     *primary_limit(const readable<double> &C) const throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const actors &);
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned  molecularity; //!< molecularity = sum_i nu_i
            const int       algebraic_Z;  //!< algebraic_Z  = sum_i nu_i z_i
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            cxx_list_of<actor>                crew; //!< actors
            mutable memory::workplace<xlimit> wksp; //!< data for xlimit
        };

    }

}

#endif
