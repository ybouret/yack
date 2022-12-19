//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"
#include "yack/data/set.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/container/matrix.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/system/imported.hpp"
#include "yack/sequence/cxx-array.hpp"

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
        typedef cxx_list_of<qfamily> qfamilies; //!< alias


        //______________________________________________________________________
        //
        //
        //! for sanity checks
        //
        //______________________________________________________________________
#define YACK_RAVEN_CHECK(PTR)   assert((PTR)->basis->size+(PTR)->ready->size==mu.rows); assert((PTR)->basis.excludes((PTR)->ready))

        //______________________________________________________________________
        //
        //
        //! family built from a set of vectors
        //
        //______________________________________________________________________
        class qfamily : public object, public qmetrics
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
            qmetrics(mu.cols),
            next(0),
            prev(0),
            qbase( new qmatrix(dimension,rk) ),
            basis(io),
            ready(io)
            {
                //--------------------------------------------------------------
                // initialize input
                //--------------------------------------------------------------
                const size_t nr = mu.rows;  assert(nr>=2);
                const size_t ir = id[nr];   assert(ir>=1); assert(ir<=mu.rows);

                //--------------------------------------------------------------
                // initialize matrix
                //--------------------------------------------------------------
                qmatrix     &Q  = *qbase;
                if(!Q(mu[ir])) throw imported::exception("raven::qfamily","invalid first vector[%u]", static_cast<unsigned>(ir));

                //--------------------------------------------------------------
                // initialize indices
                //--------------------------------------------------------------
                basis += ir;                             // basis
                for(size_t i=1;i<nr;++i) ready += id[i]; // ready

                //--------------------------------------------------------------
                // sanity check
                //--------------------------------------------------------------
                assert(1==basis->size);
                assert(nr-1==ready->size);
                YACK_RAVEN_CHECK(this);
            }
            
            //! make a hard full copy
            qfamily(const qfamily &);
            
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const qfamily &); //!< display with all info
            const qmatrix &       operator*()  const throw();                  //!< access, const
            const qmatrix *       operator->() const throw();                  //!< access, const
            qmatrix       *       operator->()       throw();                  //!< access
            qmatrix       &       operator*()        throw();                  //!< access

            //__________________________________________________________________
            //
            //! generate all possible lineage from this family
            /**
             \param lineage target lineage of new possible families
             \param mu      row matrix of usable vectors
             */
            //__________________________________________________________________
            template <typename T> inline
            bool generate(list_of<qfamily> &lineage,
                          const matrix<T>  &mu) const
            {
                YACK_RAVEN_CHECK(this);
                if(ready->size<=0) return false;

                switch(qbase->active_state)
                {
                    case qmatrix::meaningless: // empty, say goodbay
                        break;
                        
                    case qmatrix::fully_grown: // fully grown, no more children
                        break;
                        
                    case qmatrix::almost_done: // at most one new child
                        finish(lineage,mu);
                        break;
                        
                    case qmatrix::in_progress: // make new families
                        expand(lineage,mu);
                        return ready->size>0;
                }

                return false;
            }

            //__________________________________________________________________
            //
            //! test that all ready vector produces the same output
            //__________________________________________________________________
            template <typename T> inline
            bool consistent_almost_done(const matrix<T> &mu) const
            {
                assert(qbase->active_state==qmatrix::almost_done);
                qmatrix          &Q       = coerce(**this);
                const qNode      *node    = ready->head;
                cxx_array<apz>    primary(dimension);
                cxx_array<apz>    replica(dimension);
                while(node)
                {
                    if(Q.guess(primary,mu[**node]))
                        break; //! found primary
                    node=node->next;
                }

                for(node=node->next;node;node=node->next)
                {
                    if(Q.guess(replica,mu[**node])) {
                        if( replica !=  primary ) return false;
                    }
                }

                return true;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            qfamily           *next;  //!< for list
            qfamily           *prev;  //!< for list

        private:
            YACK_DISABLE_ASSIGN(qfamily);
            clone_ptr<qmatrix> qbase; //!< current base
            qList              basis; //!< indices of vectors employed  in basis
            qList              ready; //!< indices of vectors available in ready

            //__________________________________________________________________
            //
            //
            // this is the last stage:
            // any input vector produces 0 or the same output vector
            //
            //__________________________________________________________________
            template <typename T>
            void finish(list_of<qfamily> &lineage,
                        const matrix<T>  &mu) const
            {
                assert(qmatrix::almost_done==qbase->active_state);
                assert( consistent_almost_done(mu) );

                //--------------------------------------------------------------
                // full copy of this
                //--------------------------------------------------------------
                qfamily          *f = new qfamily(*this);
                auto_ptr<qfamily> F = f;
                qmatrix          &Q = **F;

                //--------------------------------------------------------------
                // look up for a new vector from ready indices
                //--------------------------------------------------------------
                while(F->ready->size)
                {
                    const size_t i = F->ready.pull_lower();
                    F->basis.ensure(i);
                    if(Q(mu[i]))
                    {
                        //------------------------------------------------------
                        // found one = last
                        //------------------------------------------------------
                        assert(qmatrix::fully_grown==Q.active_state);
                        F->basis.merge(F->ready);
                        F->ready.free();
                        YACK_RAVEN_CHECK(f);
                        std::cerr << "finish: " << F << std::endl;
                        lineage.push_back( F.yield() );
                        return;
                    }
                    //----------------------------------------------------------
                    // else mu[i] is in linear space
                    //----------------------------------------------------------
                }
            }

            //__________________________________________________________________
            //
            //
            // expand before last stage
            //
            //__________________________________________________________________
            template <typename T> inline
            void expand(list_of<qfamily> &lineage,
                        const matrix<T>  &mu) const
            {
                assert(ready->size>0);
                //std::cerr << "==> Expanding " << *this << " <==" << std::endl;


                qList             span(basis.cache); // preparing a list of spanned indice
                {
                    //----------------------------------------------------------
                    //
                    // start from a  fist clone and a first index from ready
                    //
                    //----------------------------------------------------------
                    auto_ptr<qfamily> chld = new qfamily(*this);
                    const qNode      *node = ready->head;
                    
                NEXT_CHILD:
                    const size_t      i = **node; assert(i>=1); assert(i<=mu.rows);
                    qmatrix          &Q = **chld; assert(Q==**this);
                    if(Q(mu[i]))
                    {
                        //------------------------------------------------------
                        // found a new vector
                        //------------------------------------------------------
                        chld->basis.ensure(i);
                        lineage.push_back( chld.yield() );
                        if(NULL!=(node=node->next))
                        {
                            chld = new qfamily(*this); //-----------------------
                            goto NEXT_CHILD;           // again with new child
                        }                              //-----------------------
                    }
                    else
                    {
                        //------------------------------------------------------
                        // this vector is in span
                        //------------------------------------------------------
                        span.ensure(i);
                        if(NULL!=(node=node->next))
                        {                    //---------------------------------
                            goto NEXT_CHILD; // again with same child
                        }                    //---------------------------------
                    }
                }

                //--------------------------------------------------------------
                //
                // finalize families by updating indidces
                //
                //--------------------------------------------------------------
                for(qfamily *f=lineage.head;f;f=f->next)
                {
                    f->basis += span;
                    f->ready -= f->basis;
                    std::cerr << "    expanded: " << *f << std::endl;
                    YACK_RAVEN_CHECK(f);
                }

                //--------------------------------------------------------------
                //
                // reduce families with the same last index
                //
                //--------------------------------------------------------------
                reduce(lineage,mu);
            }
            

            //------------------------------------------------------------------
            //
            //! reducing lineage with same ancestor family
            /**
             The only difference is the last vector
             */
            //------------------------------------------------------------------
            template <typename T>
            static void reduce(list_of<qfamily> &lineage,
                               const matrix<T>  &mu)
            {
                qfamilies   accepted;
                while(lineage.size)
                {
                    //----------------------------------------------------------
                    // take current family
                    //----------------------------------------------------------
                    auto_ptr<qfamily> f = lineage.pop_front();
                    qmatrix          &F = **f;
                    bool              reduced = false;

                    //----------------------------------------------------------
                    // scan already accepted distinct families (siblings)
                    //----------------------------------------------------------
                    for(qfamily * g =accepted.head;g;g=g->next) {
                        qmatrix & G = **g; assert(qmatrix::are_siblings(F,G));
                        if( F.last() == G.last() ) {
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
            
            
            // check the source basis vector are included in target matrix
            template <typename T> static
            inline bool compatible_basis(qfamily         &target,
                                         const qfamily   &source,
                                         const matrix<T> &mu)
            {
                qmatrix       &Q = *target;
                // check compatibilty of source basis with target matrix
                for(const qNode *node = source.basis->head;node;node=node->next)
                {
                    if(!Q.includes(mu[**node])) return false;
                }
                return true;
            }

        public:
            
            //------------------------------------------------------------------
            //
            //! collapsing equal or equivalent families
            //
            //------------------------------------------------------------------
            template <typename T> static inline
            void collapse(qfamily         &target,
                          qfamily         &source,
                          const matrix<T> &
#if !defined(NDEBUG)
                          mu
#endif
            )
            {
                // sanity check
                assert(target->current_rank==source->current_rank);
                assert(compatible_basis(target,source,mu));
                assert(compatible_basis(source,source,mu));

                // fusion
                target.basis += source.basis; // merge basis
                target.ready += source.ready; // merge ready
                target.ready -= target.basis; // cleanup ready
                //std::cerr << "    collapse: " << target << std::endl;
                YACK_RAVEN_CHECK(&target);
            }






        };

    }

}

#endif

