
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
        typedef suffix_set<string,const component::pointer> components_set;
        typedef components_set::knot_type                   cnode;
        
        class components
        {
        public:
            explicit components() throw();
            virtual ~components() throw();

            const cnode *head() const throw();
            size_t       size() const throw();

            bool add(const species &sp,
                     const int      nu);

            double         mass_action(const double K, const readable<double> &C) const throw();
            std::ostream & display(std::ostream &os) const;

            

            const actors  reac;
            const actors  prod;
            const int     d_nu;


            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
            components_set db;
            void update() throw();

        };

    }

}

#endif
