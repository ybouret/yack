//! \file

#ifndef YACK_CHEMICAL_UDICT_INCLUDED
#define YACK_CHEMICAL_UDICT_INCLUDED 1

#include "yack/chem/indexed.hpp"
#include "yack/associative/suffix/map.hpp"
#include "yack/large-object.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {

    namespace chemical {
        
        typedef suffix_map<size_t,size_t> umap_; //!< alias

        //______________________________________________________________________
        //
        //
        //! single look up table
        //
        //______________________________________________________________________
        class umap: public large_object, public umap_
        {
        public:
            typedef auto_ptr<const umap> ptr; //!< alias
            explicit umap() throw();          //!< setup
            virtual ~umap() throw();          //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(umap);
        };

        //______________________________________________________________________
        //
        //
        //! dictionay: forward and reverse look up tables
        //
        //______________________________________________________________________
        class udict : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const udict> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit udict(const char *which); //!< setup with name
            virtual ~udict() throw();          //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! build tables form an initial sub_node
            template <typename NODE>
            void record(const NODE *node) {
                for(;node;node=node->next)
                {
                    const indexed &x = node->host;
                    const size_t   I = *x;     //!< global
                    const size_t   i = **node; //!< local
                    if( !coerce(*fwd).insert(I,i) ) throw_multiple_forward(I);
                    if( !coerce(*rev).insert(i,I) ) throw_multiple_reverse(i);
                }
            }

            size_t forward(const size_t) const; //!< global to local
            size_t reverse(const size_t) const; //!< local to global

            //! expand source into target using reverse mapping
            template <typename TARGET, typename SOURCE> inline
            void expand(TARGET &target, SOURCE &source) const {
                for(size_t I=target.size();I>0;--I) target[I] = 0;
                for(size_t i=source.size();i>0;--i) target[ reverse(i) ] = source[i];
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udict);
            const umap::ptr fwd;
            const umap::ptr rev;
            char            kind[64 - (2*sizeof(umap::ptr)+sizeof(object))];
            void throw_multiple_forward(const size_t ) const;
            void throw_multiple_reverse(const size_t ) const;


        };


    }

}

#endif

