//! \file

#ifndef YACK_NORTH_QBRANCH_INCLUDED
#define YACK_NORTH_QBRANCH_INCLUDED 1

#include "yack/apex/north/qfamily.hpp"

namespace yack
{
    namespace north
    {

        //______________________________________________________________________
        //
        //
        //! branch of families for iteravie orthogonality construction
        //
        //______________________________________________________________________
        class qbranch
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qbranch();         //!< setup with shared I/O for indices
            virtual ~qbranch() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display metrics and one matrix per line
            friend std::ostream & operator<<(std::ostream &, const qbranch &);

            //! boot a new configuration, depth must be 0 or 1
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

            //! iterate to next depth
            /**
             - maximum depth is rank(vbase)
             - returning 0 means end of algorithm
             */
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

            //! release memory
            void prune() throw();

            //! algorithm on each newly created vector
            template <typename FUNC> inline
            void for_each( FUNC &func ) const
            {
                assert(check_depth());
                for(const qfamily *member=qlist.head;member;member=member->next)
                {
                    const qmatrix       &Q = **member;
                    const readable<apq> &q = Q[depth];
                    func(q);
                }
            }



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t       depth; //!< automatic depth monitoring
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            qfamily::list_type qlist;
            qidx_bptr          idxIO;

            bool found_twins() const throw(); //!< check, mostly for debug
            bool check_depth() const throw(); //!< check, mostly for debug
            
        };

    }

}

#endif