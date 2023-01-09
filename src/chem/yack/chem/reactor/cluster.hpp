
#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {
    
    namespace chemical {
        
        class cluster : public object
        {
        public:
            explicit cluster() throw();
            virtual ~cluster() throw();
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };
        
    }
    
}

#endif

