
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

        class qbranch
        {
        public:
            explicit qbranch();
            virtual ~qbranch() throw();

            void prune() throw();

            const list_of<qfamily> & operator*()  const throw();
            const list_of<qfamily> * operator->() const throw();
            friend std::ostream & operator<<(std::ostream &os, const qbranch &);

            //! initialize branch
            template <typename T, typename FUNC> inline
            void operator()(const matrix<T> &mu,
                            const size_t     rk,
                            FUNC            &ok)
            {
                // cleanup
                prune();
                try {
                    // prepare data
                    const size_t      nr = mu.rows;
                    cxx_array<size_t> id(nr); id.ld_incr(1);

                    // loop over all first vector
                    for(size_t i=1;i<=nr;++i)
                    {
                        rolling::down(id); assert(i==id[nr]);
                        if(!ok(mu[i])) continue;
                        qlist.push_back( new qfamily(id,mu,rk,io) );
                        coerce(depth) = 1;
                    }
                }
                catch(...) { prune(); throw; }
            }




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

                        // generate original lineage, no duplicate
                        parents->generate(lineage,mu);

                        // present all new vectors to callback
                        std::cerr << "==> +" << lineage.size << std::endl;
                        for(const qfamily *f=lineage.head;f;f=f->next)
                            cb( (**f).last() );

                        // condense lineage
                        condense(lineage,mu);




                        // fusion
                        target.merge_back(lineage);
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
            
            const size_t depth;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
            qfamilies qlist;

        public:
            qFund io;

        private:
            template <typename T>
            static void condense(list_of<qfamily> &lineage,
                                 const matrix<T>  &mu)
            {
                qfamilies surrogate;
                while(lineage.size)
                {
                    auto_ptr<qfamily> f = lineage.pop_front();
                    qmatrix          &F = **f;
                    bool              squeezed = false;
                    for(qfamily  *g=surrogate.head;g;g=g->next)
                    {
                        qmatrix &G = **g;
                        if( F==G || F.is_equivalent_to(G) )
                        {
                            std::cerr << "condense " << F << " and " << G << std::endl;
                            exit(0);
                            collapse(*g,*f,mu);
                            squeezed = true;
                            break;
                        }
                    }

                    if(squeezed) continue;             // drop f
                    surrogate.push_back( f.yield() ); // keep f
                }
                lineage.swap_with(surrogate);
            }
            
        };

    }

}

#endif

