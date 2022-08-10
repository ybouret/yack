
//! \file

#ifndef YACK_CHEMICAL_COMPONENTS_INCLUDED
#define YACK_CHEMICAL_COMPONENTS_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/chem/component.hpp"
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
            static const char clid[];
            
            explicit components() throw();
            virtual ~components() throw();
            
            
            void operator()(const species &sp,
                            const int      nu);
            
            
            const cnode *head() const throw();
            
            friend std::ostream & operator<<(std::ostream &, const components &);
            
            const actors reac;
            const actors prod;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
            components_set cdb;
            
        };
        
    }
    
}

#endif
