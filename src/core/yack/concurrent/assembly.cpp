#include "yack/concurrent/assembly.hpp"

namespace yack
{
    namespace concurrent
    {

        assembly:: ~assembly() throw()
        {
        }

        assembly:: assembly() throw() : readable<context>()
        {
        }

        
        void assembly:: ensure(const size_t num_blocks, const size_t block_size) const
        {
            const readable<context> &self = *this;
            const size_t             nctx = self.size();
            for(size_t i=1;i<=nctx;++i)
            {
                const context &ctx = self[i];
                (*coerce(ctx)).ensure(num_blocks,block_size);
            }
        }


        void assembly:: free_local_memories() throw()
        {
            const readable<context> &self = *this;
            const size_t             nctx = self.size();
            for(size_t i=1;i<=nctx;++i)
            {
                const context &ctx = self[i];
                coerce(ctx).free_local_memory();
            }
        }

        void assembly:: drop_local_memories() throw()
        {
            const readable<context> &self = *this;
            const size_t             nctx = self.size();
            for(size_t i=1;i<=nctx;++i)
            {
                const context &ctx = self[i];
                coerce(ctx).drop_local_memory();
            }
        }

    }
}
