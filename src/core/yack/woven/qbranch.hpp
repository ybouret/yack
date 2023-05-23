
//! \file

#ifndef YACK_WOVEN_QBRANCH_INCLUDED
#define YACK_WOVEN_QBRANCH_INCLUDED 1

#include "yack/woven/qfamily.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/woven/zrepo.hpp"

namespace yack
{
    namespace woven
    {
        //______________________________________________________________________
        //
        //! base class for qbranch
        //______________________________________________________________________
        typedef cxx_list_of<qfamily>     qfamilies;

        //______________________________________________________________________
        //
        //
        //! list of distinct families
        //
        //______________________________________________________________________
        class qbranch : public metrics, public qfamilies
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< "qbranch"
            static bool       doReduce;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qbranch(const size_t dims, const indices::fund &); //!< setup with shared memory
            virtual ~qbranch() noexcept;                                //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! initialize families with each row of data
            template <typename T> inline
            void initialize(const matrix<T> &data,
                            zrepo           &repo,
                            const bool       load)
            {
                initialize( check_ndof(data) );
                repo.release();
                for(qfamily *f=head;f;f=f->next)
                {
                    if(!f->try_grow(data)) raise_null();
                    if(load) repo.ensure(*f);
                }
            }
            

            //! display, mostly to debug
            friend std::ostream & operator<<(std::ostream &, const qbranch &);



            //! compute next generation
            template <typename T> inline
            void generate(const qbranch    &ancestors,
                          const matrix <T> &data,
                          const size_t      rank,
                          zrepo            &repo)
            {
                assert( this != &ancestors );
                release();
                for(const qfamily *F=ancestors.head;F;F=F->next)
                {
                    generate(*F,data,rank,repo);
                }

            }

            void collect( zrepo &repo ) const;
            

            indices::fund fund; //!< shared memory for indices

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            size_t check_ndof(const matrix_metrics &M) const; //!< check right dimensions and return ndof
            void   initialize(const size_t ndof);             //!< initialize all families
            void   raise_null() const;                        //!< found null vector
            void   raise_greater_rank() const;                //!< invalid matrix rank
            void   raise_smaller_rank() const;                //!< invalid matrix rank

            //! incremental generation from ONE family of parents
            /**
             \param parents a family of qvectors
             \param data    rows of vectors with same dimensions
             \param rank    rank of data
             */
            template <typename T> inline
            void generate(const qfamily    &parents,
                          const matrix <T> &data,
                          const size_t      rank,
                          zrepo            &repo)
            {
                assert(rank>0);
                assert(rank<=min_of(data.cols,data.rows));
                assert(parents.size>0);
                assert(parents.size<=rank);

                //--------------------------------------------------------------
                //
                // check where we are
                //
                //--------------------------------------------------------------
                switch(rank-parents.size)
                {
                    case 0:{
                        //------------------------------------------------------
                        // all done: check all remaining vectors are contained
                        //------------------------------------------------------
                        assert(rank==parents.size);
                        auto_ptr<qfamily> lineage = new qfamily(parents);
                        while(lineage->try_grow(data)) raise_greater_rank();
                    } return;

                    case 1:
                    {
                        //------------------------------------------------------
                        // any indep vector shall produce the same output
                        //------------------------------------------------------
                        assert(rank-1==parents.size);
                        auto_ptr<qfamily> lineage = new qfamily(parents);
                    BUILD_LAST:
                        if(lineage->try_grow(data)) { assert(rank==lineage->size); // found last vector
                            while(lineage->try_grow(data)) raise_greater_rank();   // lineage should have max rank
                            repo.ensure( *push_back( lineage.yield() ) );          // push back lineage and update repo
                            return;                                                // and return without compression
                        }
                        if(lineage->indx.size<=0) raise_smaller_rank();
                        goto BUILD_LAST;
                    }


                    default:
                        assert(parents.size<rank-1);
                        break;
                }

                //--------------------------------------------------------------
                //
                // build all possible families
                //
                //--------------------------------------------------------------
                qfamilies children;

                {
                    const size_t      siblings = parents.indx.size;
                    size_t            rotation = 0;
                    auto_ptr<qfamily> lineage = new qfamily(parents);

                LINEAGE:
                    if(lineage->try_grow(data))
                    {
                        repo.ensure(*children.push_back( lineage.yield() ));
                        if(++rotation<siblings)
                        {
                            lineage = new qfamily(parents);
                            for(size_t k=rotation;k>0;--k) lineage->indx.roll_down();
                            goto LINEAGE;
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // children count reduction
                //
                //--------------------------------------------------------------
                if(doReduce)
                {
                    qfamilies sampling;
                    reduce(sampling,children);
                    reduce(*this,sampling);
                }
                else
                {
                    merge_back(children);
                }


            }

            static void reduce(qfamilies &);
            static void reduce(qfamilies &lhs, qfamilies &rhs);


        };

    }
}

#endif
