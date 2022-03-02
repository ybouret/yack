
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/component.hpp"
#include "yack/chem/actors.hpp"
#include "yack/associative/suffix/set.hpp"


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
            // methods
            //__________________________________________________________________
            const cnode *head() const throw(); //!< first component
            size_t       size() const throw(); //!< number of components

            //! append a new component, false if species was already registered
            bool operator()(const species &sp,
                            const int      nu);


            //! reac.mass_action(K,C) - prod.mass_action(1,C)
            double         mass_action(const double K, const readable<double> &C) const throw();

            //! helper to display
            std::ostream & display(std::ostream &os) const;

            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors  reac; //!< list of reactants
            const actors  prod; //!< list of products
            const int     d_nu; //!< Delta_r nu


            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
            components_set db;
            void update() throw();

        };

    }

}

#endif
