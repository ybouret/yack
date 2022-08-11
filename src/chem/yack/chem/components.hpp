
//! \file

#ifndef YACK_CHEMICAL_COMPONENTS_INCLUDED
#define YACK_CHEMICAL_COMPONENTS_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/chem/component.hpp"
#include "yack/chem/xlimits.hpp"
#include "yack/associative/suffix/set.hpp"

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
            static const char clid[]; //!< components
           
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
            
            //! display reac <=> prod
            friend std::ostream & operator<<(std::ostream &, const components &);

            //! compute mass action for a given constant
            double mass_action(const double            K,
                               const readable<double> &C,
                               rmulops                &ops) const;
            
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
