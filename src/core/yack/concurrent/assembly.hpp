
//! \file

#ifndef YACK_SYNC_ASSEMBLY_INCLUDED
#define YACK_SYNC_ASSEMBLY_INCLUDED 1

#include "yack/concurrent/context.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/groove.hpp"

namespace yack
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! assembly of contexts
        //
        //______________________________________________________________________
        class assembly : public readable<context>
        {
        public:
            //! cleanup
            virtual ~assembly() throw();

            //! free all local memories
            void free_local_memories() throw();

            //! drop all local memories
            void drop_local_memories() throw();
            
            //! ensuring num_blocks of block_size per context
            void ensure(const size_t num_blocks, const size_t block_size) const;


            //! building of T[0:n-1] for each context
            template <typename T> inline
            void build(const size_t n) const {
                const readable<context> &self = *this;
                const size_t             nctx = self.size();
                for(size_t i=1;i<=nctx;++i)
                {
                    const context &ctx = self[i];
                    (*ctx).build<T>(n);
                }
            }

            //! build one type per context
            template <typename T> inline
            void build() const {
                build<T>(1);
            }




        protected:
            //! setup
            explicit assembly() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(assembly);
        };
        
    }

}

#endif
