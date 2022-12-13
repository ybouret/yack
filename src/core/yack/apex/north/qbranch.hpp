//! \file

#ifndef YACK_NORTH_QBRANCH_INCLUDED
#define YACK_NORTH_QBRANCH_INCLUDED 1

#include "yack/apex/north/qfamily.hpp"

namespace yack
{
    namespace north
    {

        class qbranch
        {
        public:
            explicit qbranch();
            virtual ~qbranch() throw();

            friend std::ostream & operator<<(std::ostream &, const qbranch &);
            
            template <typename T> inline
            void boot(const readable<size_t> &rindx,
                      const matrix<T>        &vbase)
            {
                assert(  check_depth() );
                qlist.push_back(new qfamily(rindx,vbase,idxIO));
                coerce(depth) = 1;
                assert(  check_depth() );
                assert( !found_twins() );
            }

            template <typename T> inline
            size_t next(const matrix<T> &vbase)
            {
                qfamily::list_type target;
                qfamily::generate(target,qlist,vbase); assert(0==qlist.size);
                qlist.swap_with(target);
                if(qlist.size>0)
                {
                    ++coerce(depth);
                    assert( check_depth()  );
                    assert( !found_twins() );
                }
                else
                {
                    coerce(depth) = 0;
                }
                return depth;
            }



            bool found_twins() const throw();
            bool check_depth() const throw();

            void prune() throw();
            


            const size_t       depth;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            qfamily::list_type qlist;
            qidx_bptr          idxIO;


            
        };

    }

}

#endif
