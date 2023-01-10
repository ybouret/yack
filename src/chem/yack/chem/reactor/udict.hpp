//! \file

#ifndef YACK_CHEMICAL_UDICT_INCLUDED
#define YACK_CHEMICAL_UDICT_INCLUDED 1

#include "yack/chem/indexed.hpp"
#include "yack/associative/suffix/map.hpp"
#include "yack/large-object.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {

    namespace chemical {
        
        typedef suffix_map<size_t,size_t> umap_;

        class umap: public large_object, public umap_
        {
        public:
            typedef auto_ptr<const umap> ptr;
            explicit umap() throw();
            virtual ~umap() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(umap);
        };


        class udict : public object
        {
        public:
            typedef auto_ptr<const udict> ptr;
            
            explicit udict(const char *which);
            virtual ~udict() throw();

            template <typename NODE>
            void record(const NODE *node) {
                for(;node;node=node->next)
                {
                    const indexed &x = node->host;
                    const size_t   i = *x;
                    const size_t   I = **node;
                    if( !coerce(*fwd).insert(i,I) ) throw_multiple_forward();
                    if( !coerce(*rev).insert(I,i) ) throw_multiple_reverse();
                }
            }




            const umap::ptr fwd;
            const umap::ptr rev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udict);
            char kind[64 - (2*sizeof(umap::ptr)+sizeof(object))];
            void throw_multiple_forward() const;
            void throw_multiple_reverse() const;


        };


    }

}

#endif

