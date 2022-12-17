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

        //______________________________________________________________________
        //
        //
        // alias for families
        //
        //______________________________________________________________________
        class   qfamily;
        typedef cxx_list_of<qfamily> qfamilies;


        //! for sanity checks
#define YACK_RAVEN_CHECK(PTR)   assert((PTR)->basis->size+(PTR)->ready->size==mu.rows); assert((PTR)->basis.excludes((PTR)->ready))

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
                YACK_RAVEN_CHECK(this);
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
                YACK_RAVEN_CHECK(this);
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
                std::cerr << "==> Expanding " << *this << " <==" << std::endl;

                // preparing a list of spanned indice
                qList             span(basis.cache);
                {
                    auto_ptr<qfamily> chld = new qfamily(*this);
                    const qNode      *node = ready->head;

                NEXT_CHILD:
                    const size_t      i = **node;
                    qmatrix          &Q = *(chld->qbase);
                    if(Q(mu[i]))
                    {
                        // found a new vector
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
                        // this vector is in span
                        span.ensure(i);
                        if(NULL!=(node=node->next))
                            goto NEXT_CHILD; // with same chld
                    }
                }

                for(qfamily *f=lineage.head;f;f=f->next)
                {
                    f->basis += span;
                    f->ready -= f->basis;
                    std::cerr << "    expanded: " << *f << std::endl;
                    YACK_RAVEN_CHECK(f);
                }

                reduce(lineage,mu);

            }


            //! reducing lineage with same ancestor family
            /**
             The only difference is the last vector
             */
            template <typename T>
            static void reduce(list_of<qfamily> &lineage,
                               const matrix<T>  &mu)
            {
                const size_t ini = lineage.size;
                {
                    qfamilies   accepted;
                    while(lineage.size)
                    {
                        auto_ptr<qfamily> f = lineage.pop_front();
                        qmatrix          &F = **f;
                        bool              reduced = false;
                        for(qfamily  *g=accepted.head;g;g=g->next)
                        {
                            qmatrix &G = **g;
                            if( F.last() == G.last() )
                            {
                                collapse(*g,*f,mu);
                                reduced = true;
                                break;
                            }
                        }

                        if(reduced) continue;            // drop f
                        accepted.push_back( f.yield() ); // keep f
                    }
                    lineage.swap_with(accepted);
                }
                const size_t end = lineage.size;
                assert(end<=ini);
                if(end<ini)
                {
                    std::cerr << "reduced: " << ini << " -> " << end << std::endl;
                }

            }


            static void throw_distinct_basis();

        public:
            template <typename T> static inline
            void collapse(qfamily         &target,
                          qfamily         &source,
                          const matrix<T> &mu)
            {
                qmatrix &tgt = *target;
                qmatrix &src = *source;

                assert(tgt.current_rank==src.current_rank);

                // check compatibility of target basis with source matrix
                for(const qNode *node = target.basis->head;node;node=node->next)
                {
                    const size_t i = **node;
                    if(!src.includes(mu[i])) throw_distinct_basis();
                }

                // check compatibilty of source basis with target matrix
                for(const qNode *node = source.basis->head;node;node=node->next)
                {
                    const size_t i = **node;
                    if(!tgt.includes(mu[i])) throw_distinct_basis();
                }

                // fusion!
                target.basis += source.basis;
                target.ready += source.ready;
                target.ready -= target.basis;
                std::cerr << "    collapse: " << target << std::endl;
                YACK_RAVEN_CHECK(&target);
            }






        };

    }

}

#endif

