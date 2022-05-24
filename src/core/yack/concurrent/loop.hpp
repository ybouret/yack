//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/concurrent/loop/runnable.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/groove.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! interface for Single Instruction Multiple Data
        //
        //______________________________________________________________________
        class loop : public object, public counted, public readable<context>
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual lockable   &access()          throw() = 0; //!< to sync
            virtual const char *family()    const throw() = 0; //!< for info
            virtual void operator()(kernel,void*) throw() = 0; //!< call kernel once

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            void run(runnable &obj) throw(); //!< obj.run(...)

            //! compute efficiency
            double efficiency(const double seq_rate, const double par_rate) const throw();

            //__________________________________________________________________
            //
            // data management
            //__________________________________________________________________

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

            //! free all local memories
            void free_local_memories() throw();

            //! drop all local memories
            void drop_local_memories() throw();


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~loop() throw(); //!< cleanup
        protected:
            explicit loop() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
