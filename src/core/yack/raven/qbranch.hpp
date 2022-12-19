
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
        //! branch of distinct families with generation monitoring
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
            // generating methods
            //__________________________________________________________________

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
            void operator()(const matrix<T> &mu,
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
                        coerce(depth) = 1;
                    }
                }
                catch(...) { prune(); throw; }
            }



            //__________________________________________________________________
            //
            //! initialize branch
            /**
             \param mu matrix of row vectors
             \param cb call on generated new qvector
             */
            //__________________________________________________________________
            template <typename T, typename PROC>
            inline size_t generate(const matrix<T> &mu,
                                   PROC            &cb)
            {
                std::cerr << "-------- Branching @depth=" << depth << " #" << qlist.size << " --------" << std::endl;
                {
                    qfamilies target;
                    while(qlist.size)
                    {
                        const auto_ptr<qfamily> parents = qlist.pop_front();
                        qfamilies               lineage;

                        // generate reduced lineage, no duplicate
                        const bool has_future = parents->generate(lineage,mu);

                        // present all new vectors to callback
                        if(lineage.size)
                        {
                            for(const qfamily *f=lineage.head;f;f=f->next)
                                cb( (**f).last() );

                            //std::cerr << "has_future=" << has_future << std::endl;

                            if(has_future)
                            {
                                intra_condensation(lineage,mu);         // condense lineage
                                incremental_fusion(target,lineage,mu);  // incremental compact
                            }
                            else
                            {
                                lineage.release();
                                //intra_condensation(lineage,mu);         // condense lineage
                                //incremental_fusion(target,lineage,mu);  // incremental compact
                            }

                        }
                    }
                    target.swap_with(qlist);
                }
                
                const size_t nf = qlist.size;
                if(nf)
                {
                    ++coerce(depth);
                    return nf;
                }
                else
                {
                    coerce(depth) = 0;
                    return 0;
                }
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t depth; //!< depth monitoring

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
                        //std::cerr << "    condense: " << candidate << " and " << *g << std::endl;
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
                // no equivalent found at this point...
                //
                //--------------------------------------------------------------
                surrogate.push_back( candidate.yield() );
            }



            template <typename T> static inline
            void intra_condensation(list_of<qfamily> &lineage,
                                    const matrix<T>  &mu)
            {
                qfamilies surrogate;
                while(lineage.size)
                {
                    auto_ptr<qfamily> candidate = lineage.pop_front();
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

