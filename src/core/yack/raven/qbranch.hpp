
//! \file

#ifndef YACK_RAVEN_QBRANCH_INCLUDED
#define YACK_RAVEN_QBRANCH_INCLUDED 1

#include "yack/raven/qfamily.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/roll.hpp"

namespace yack
{
    namespace raven
    {

        //______________________________________________________________________
        //
        //
        //! branch containing iterative generations
        //
        //______________________________________________________________________
        class qbranch
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qbranch();           //!< setup empty
            virtual ~qbranch() throw();   //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                     prune()            throw();                  //!< remove all families
            const list_of<qfamily> & operator*()  const throw();                  //!< access, const
            const list_of<qfamily> * operator->() const throw();                  //!< access, const
            friend std::ostream    & operator<<(std::ostream &, const qbranch &); //!< display

            //__________________________________________________________________
            //
            //! initialize branch
            /**
             \param mu matrix of row vectors
             \param rk rank(mu)
             \param ok keep mu[i] iff ok(mu[i])
             */
            //__________________________________________________________________
            template <typename T, typename FUNC> inline
            size_t init(const matrix<T> &mu,
                        const size_t     rk,
                        FUNC            &ok)
            {
                //--------------------------------------------------------------
                // cleanup
                //--------------------------------------------------------------
                prune();
                try {
                    //----------------------------------------------------------
                    // prepare data: number of rows and indices
                    //----------------------------------------------------------
                    const size_t      nr = mu.rows;
                    cxx_array<size_t> id(nr); id.ld_incr(1);

                    //----------------------------------------------------------
                    // loop over all first vector
                    //----------------------------------------------------------
                    for(size_t i=1;i<=nr;++i) {
                        rolling::down(id); assert(i==id[nr]);
                        if(!ok(mu[i])) continue;
                        qlist.push_back( new qfamily(id,mu,rk,io) );
                    }
                    return qlist.size;
                }
                catch(...) { prune(); throw; }
            }

            //__________________________________________________________________
            //
            //! initialize branch
            /**
             \param mu matrix of row vectors
             \param rk rank(mu)
             \param cb processing new qvector
             */
            //__________________________________________________________________
            template <typename T, typename PROC> inline
            size_t init0(const matrix<T> &mu,
                         const size_t     rk,
                         PROC            &cb)
            {
                //--------------------------------------------------------------
                // cleanup
                //--------------------------------------------------------------
                prune();
                try {
                    //----------------------------------------------------------
                    // prepare data: number of rows and indices
                    //----------------------------------------------------------
                    const size_t      nr = mu.rows;
                    cxx_array<size_t> id(nr); id.ld_incr(1);

                    //----------------------------------------------------------
                    // loop over all first vector
                    //----------------------------------------------------------
                    for(size_t i=1;i<=nr;++i) {
                        rolling::down(id); assert(i==id[nr]);
                        qfamily &f = *qlist.push_back( new qfamily(id,mu,rk,io) );
                        cb( f->last() );
                    }
                    return qlist.size;
                }
                catch(...) { prune(); throw; }
            }



            


            //__________________________________________________________________
            //
            //! grow branch
            /**
             \param mu matrix of row vectors
             \param cb call on generated new qvector
             \return number of still growing families
             */
            //__________________________________________________________________
            template <typename T, typename PROC>
            inline size_t grow(const matrix<T> &mu,
                               PROC            &cb)
            {
                {
                    //----------------------------------------------------------
                    //
                    // accumulate new generation in progeny
                    //
                    //----------------------------------------------------------
                    qfamilies progeny;
                    while(qlist.size)
                    {
                        //------------------------------------------------------
                        //
                        // loop over parents from this
                        //
                        //------------------------------------------------------
                        const auto_ptr<qfamily> parents = qlist.pop_front();
                        {
                            //--------------------------------------------------
                            // generate direct, univocal lineage
                            //--------------------------------------------------
                            qfamilies           lineage;
                            parents->generate(lineage,mu);
                            if(!lineage.size) continue;

                            //--------------------------------------------------
                            // present all new vectors to callback
                            //--------------------------------------------------
                            for(const qfamily *f=lineage.head;f;f=f->next)
                                cb( (**f).last() );

                            intra_condensation(lineage,mu);          // condense lineage
                            incremental_fusion(progeny,lineage,mu);  // incremental fusion
                        }
                        //------------------------------------------------------
                        //
                        // progeny was updated
                        //
                        //------------------------------------------------------
                    }
                    progeny.swap_with(qlist);
                }
                
                return qlist.size;
            }

            //! full batch processing
            template <typename T,
            typename CONFIRM,
            typename PROCESS> inline
            void batch(const matrix<T> &mu,
                       const size_t     rk,
                       CONFIRM         &ok,
                       PROCESS         &cb)
            {
                if( init(mu,rk,ok) ) {
                    while( grow(mu,cb) )
                        ;
                }
            }

            template <typename T, typename PROCESS> inline
            void batch(const matrix<T> &mu,
                       const size_t     rk,
                       PROCESS         &cb)
            {
                if(init0(mu,rk,cb)) {
                    while( grow(mu,cb) )
                        ;
                }
            }

            


            //__________________________________________________________________
            //
            //! helper to assess new coefficients: summed = weight'*nu
            //__________________________________________________________________
            template <typename T, typename U,typename V> static inline
            void assess(writable<T>       &summed,
                        const readable<U> &weight,
                        const matrix<V>   &nu)
            {
                assert(summed.size()==nu.cols);
                assert(weight.size()==nu.rows);
                const size_t n = weight.size();
                for(size_t j=summed.size();j>0;--j)
                {
                    V sum = 0;
                    for(size_t i=n;i>0;--i) sum += weight[i] * nu[i][j];
                    summed[j] = sum;
                }
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            qfamilies qlist;   //!< current generations

        public:
            qFund        io;   //!< shared memory for indices

        private:
            template <typename T> static inline
            void try_merge(list_of<qfamily>  &surrogate,
                           auto_ptr<qfamily> &candidate,
                           const matrix<T>   &mu)
            {

                //--------------------------------------------------------------
                //
                // current family
                //
                //--------------------------------------------------------------
                qfamily &f = *candidate; assert(NULL==f.next); assert(NULL==f.prev);
                qmatrix &F = *f;


                //--------------------------------------------------------------
                //
                // loop over surrogate
                //
                //--------------------------------------------------------------
                for(qfamily *g = surrogate.head;g;g=g->next)
                {
                    const qmatrix &G = **g;
                    if( F==G /* fast */ || F.is_equivalent_to(G) /* slow */) {
                        //------------------------------------------------------
                        // found equivalent: keep lightest matrix
                        //------------------------------------------------------
                        if(G.total_weight <= F.total_weight)
                        {
                            // keep g, drop f=candidate
                            qfamily::collapse(*g,f,mu);
                            return; // drop f=candidate
                        }
                        else
                        {
                            // keep f=candidate, delete g
                            qfamily::collapse(f,*g,mu);
                            delete surrogate.replace(g,candidate.yield());
                            return;
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // no equivalent found at this point: enroll candidate
                //
                //--------------------------------------------------------------
                surrogate.push_back( candidate.yield() );
            }



            //! remove duplicate or complete lineage
            template <typename T> static inline
            void intra_condensation(list_of<qfamily> &lineage,
                                    const matrix<T>  &mu)
            {
                qfamilies surrogate;
                while(lineage.size) {
                    auto_ptr<qfamily> candidate = lineage.pop_front();
                    if(candidate->is_complete()) continue; // drop candidate
                    try_merge(surrogate,candidate,mu);
                }
                lineage.swap_with(surrogate);
            }


            //! compact two lists of condensed families
            template <typename T> static inline
            void incremental_fusion(list_of<qfamily> &target,
                                    list_of<qfamily> &source,
                                    const matrix<T>  &mu)
            {
                while(source.size)
                {
                    auto_ptr<qfamily> candidate = source.pop_front();
                    try_merge(target,candidate,mu);
                }
            }

            
        };

    }

}

#endif

