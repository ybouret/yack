//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"
#include "yack/data/set.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/container/matrix.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        // aliases for internal indices
        //
        //______________________________________________________________________
        typedef data_set<size_t>   qList; //!< list of indices
        typedef ds_zpool<size_t>   qBank; //!< bank of indices
        typedef ds_node<size_t>    qNode; //!< for list of indices
        typedef qBank::pointer     qFund; //!< shared bank of indices


        //! for sanity checks
#define YACK_RAVEN_CHECK()   assert(basis->size+ready->size==mu.rows); assert(basis.excludes(ready))

        //______________________________________________________________________
        //
        //
        //! family built from a set of vectors
        //
        //______________________________________________________________________
        class qfamily : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef cxx_list_of<qfamily> list_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~qfamily() throw(); //!< cleanup


            //! start a new family
            /**
             \param id indices[1...nr], id[nr] = starting index
             \param mu matrix of row vectors
             \param rk rank(mu)
             \param io cache for lists of indices
             */
            template <typename T> inline
            explicit qfamily(readable<size_t> &id,
                             const matrix<T>  &mu,
                             const size_t      rk,
                             const qFund      &io) :
            object(),
            qbase( new qmatrix(mu.cols,rk) ),
            basis(io),
            ready(io),
            next(0),
            prev(0)
            {
                // initialize input
                const size_t nr = mu.rows;  assert(nr>=2);
                const size_t ir = id[nr];   assert(ir>=1); assert(ir<=mu.rows);

                // initialize matrix
                qmatrix     &Q  = *qbase;
                if(!Q(mu[ir])) throw_singular_matrix(ir);

                // initialize indices
                basis += ir;
                for(size_t i=1;i<nr;++i) ready += id[i];

                // sanity check
                assert(1==basis->size);
                assert(nr-1==ready->size);
                YACK_RAVEN_CHECK();
            }

            //! hard copy
            qfamily(const qfamily &);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const qfamily &); //!< display

            //__________________________________________________________________
            //
            // generate all possible lineage from this family
            //__________________________________________________________________
            template <typename T> inline
            void generate(list_of<qfamily> &lineage,
                          const matrix<T>  &mu) const
            {
                YACK_RAVEN_CHECK();
                std::cerr << "generate <" << qbase->maturity_text() << ">" << std::endl;
                switch(qbase->active_state)
                {
                    case qmatrix::meaningless:
                        return;

                    case qmatrix::fully_grown:
                        return;

                    case qmatrix::almost_done:
                        // return at most one new !!
                        finish(lineage,mu);
                        return;

                    case qmatrix::in_progress:
                        expand(lineage,mu);
                        return;
                }
            }

            
            qmatrix       & operator*()       throw() { return *qbase; }
            const qmatrix & operator*() const throw() { return *qbase; }


            clone_ptr<qmatrix> qbase;
            qList              basis;
            qList              ready;
            qfamily           *next;
            qfamily           *prev;



        private:
            YACK_DISABLE_ASSIGN(qfamily);
            void throw_singular_matrix(const size_t ir) const;
            
            // this is the last stage
            template <typename T>
            void finish(list_of<qfamily> &lineage,
                        const matrix<T>  &mu) const
            {
                assert(qmatrix::almost_done==qbase->active_state);
                assert(ready->size>0);

                auto_ptr<qfamily> chld = new qfamily(*this);
                qmatrix          &Q    = *(chld->qbase);

                while(chld->ready->size)
                {
                    const size_t i = chld->ready.pull_lower();
                    chld->basis.ensure(i);
                    if(Q(mu[i]))
                    {
                        // found one = last
                        assert(qmatrix::fully_grown==Q.active_state);
                        chld->basis.merge(chld->ready);
                        chld->ready.free();
                        std::cerr << "finish: " << chld << std::endl;
                        lineage.push_back( chld.yield() );
                        return;
                    }
                }

            }


            template <typename T> inline
            void expand(list_of<qfamily> &lineage,
                        const matrix<T>  &mu) const
            {
                assert(ready->size>0);

                qList             span(basis.cache);
                {
                    auto_ptr<qfamily> chld = new qfamily(*this);
                    const qNode      *node = ready->head;

                NEXT_CHILD:
                    const size_t      i = **node;
                    qmatrix          &Q = *(chld->qbase);
                    if(Q(mu[i]))
                    {
                        chld->basis.ensure(i);
                        lineage.push_back( chld.yield() );
                        if(NULL!=(node=node->next))
                        {
                            chld = new qfamily(*this);
                            goto NEXT_CHILD; // with new child
                        }
                    }
                    else
                    {
                        span.ensure(i);
                        if(NULL!=(node=node->next))
                            goto NEXT_CHILD; // with same chld
                    }
                }

                for(qfamily *f=lineage.head;f;f=f->next)
                {
                    f->basis += span;
                    f->ready -= f->basis;
                    std::cerr << "expand: " << *f << std::endl;
                    assert(f->basis->size+f->ready->size==mu.rows);
                    assert(f->basis.excludes(f->ready));
                }

                reduce(lineage,mu);

            }

            template <typename T>
            static void reduce(list_of<qfamily> &lineage,
                               const matrix<T>  &mu)
            {
                qfamily::list_type original;
                while(lineage.size)
                {
                    auto_ptr<qfamily> F = lineage.pop_front();
                    qmatrix          &L = **F;
                    for(qfamily *G=original.head;G;G=G->next)
                    {
                        qmatrix &R = **G;
                        if(L.is_equivalent_to(R))
                        {
                            std::cerr << "sould reduce " << *F << " and " << *G << std::endl;
                            for(const qNode *node=F->basis->head;node;node=node->next)
                            {
                                
                            }
                        }
                    }
                    
                    original.push_back(F.yield());
                }
                
                lineage.swap_with(original);
                
            }


        };

    }

}

#endif

