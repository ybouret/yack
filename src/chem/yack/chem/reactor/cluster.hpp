
#ifndef YACK_CHEMICAL_CLUSTER_INCLUDED
#define YACK_CHEMICAL_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {
    
    namespace chemical {
        
        class cluster : public object
        {
        public:
            typedef cxx_list_of<cluster> list;
            explicit cluster() throw();
            virtual ~cluster() throw();
            
            cluster *next;
            cluster *prev;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cluster);
        };
        
        class clusters : public object, public cluster::list
        {
        public:
            explicit clusters() throw();
            virtual ~clusters() throw();
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
    }
    
}

#endif

