#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"

#include "yack/data/small/set.hpp"
#include "yack/sequence/roll.hpp"
#include "yack/sequence/bunch.hpp"
#include "yack/apex/worthy.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        
        
        
        namespace
        {
            class mixed_equilibrium : public equilibrium
            {
            public:
                virtual ~mixed_equilibrium() throw()
                {
                }
                
                const readable<double> &Ktab;
                rmulops                &xmul;
                const cxx_array<int>    coef;
                
                explicit mixed_equilibrium(const string           &uid,
                                           const size_t            idx,
                                           const readable<double> &myK,
                                           rmulops                &ops,
                                           const readable<int>    &arr) :
                equilibrium(uid,idx),
                Ktab(myK),
                xmul(ops),
                coef(arr,transmogrify)
                {
                    
                }
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mixed_equilibrium);
                virtual double getK(double) const
                {
                    xmul.ld1();
                    for(size_t j=coef.size();j>0;--j)
                    {
                        xmul.ipower(Ktab[j],coef[j]);
                    }
                    return xmul.query();
                }
            };
        }
        
        
        const equilibrium &nexus:: promote_mixed(const readable<int> &weight)
        {
            cxx_array<int> gcoef(M);
            for(const enode *en=singles.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const int          ew = weight[ei];
                if(!ew) continue;
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const size_t j = *****cn;
                    gcoef[j] += ew * Nu[ei][j];
                }
            }
            
            //----------------------------------------------------------
            // create a mixed equilibrium
            //----------------------------------------------------------
            equilibria  &target = coerce(lattice);
            const string name   = singles.make_name(weight);
            const size_t mxid   = target.size()+1;
            equilibrium &mxeq   = target.use( new mixed_equilibrium(name,mxid,K,xmul,weight) );
            for(size_t j=1;j<=M;++j)
            {
                const int f = gcoef[j];
                if(f) mxeq( worklib[j], f);
            }
            
            assert(mxeq.neutral());
            
            //----------------------------------------------------------
            // to register in this related group
            //----------------------------------------------------------
            return mxeq;
            
        }
        
        


        static inline size_t count_valid(const readable<int> &coef) throw()
        {
            size_t count = 0;
            for(size_t i=coef.size();i>0;--i)
            {
                if( 0 != coef[i] ) ++count;
            }
            return count;
        }

        void select_rows(imatrix       &w,
                         const imatrix &mu)
        {
            const size_t   m = mu.rows;
            vector<size_t> jrow(m,as_capacity);
            
            // find non proportional rows
            for(size_t j=1;j<=m;++j)
            {
                const readable<int> &curr = mu[j];
                if(count_valid(curr)<=0) continue;

                bool                 isOk = true;
                for(size_t k=jrow.size();k>0;--k)
                {
                    if( apk::are_prop(mu[ jrow[k] ],curr,NULL) )
                    {
                        isOk = false;
                        break;
                    }
                }
                if(isOk)
                    jrow << j;
            }
            
            // build sub-matrix
            const size_t dims = jrow.size();
            if(dims>0)
            {
                w.make(dims,mu.cols);
                for(size_t k=dims;k>0;--k)
                    iota::load(w[k],mu[ jrow[k] ]);
            }
            
        }

        
        static inline bool whole_valid(const readable<int> &arr) throw()
        {
            for(size_t i=arr.size();i>0;--i)
            {
                if( 0 == arr[i] ) return false;
            }
            return true;
        }
        
        
        static inline
        const readable<int> &q2i(writable<int>       &cof,
                                 const readable<apq> &q)
        {
            assert(cof.size()==q.size());
            for(size_t i=cof.size();i>0;--i)
            {
                assert(1==q[i].den);
                cof[i] = q[i].num.cast_to<int>();
            }
            return cof;
        }

        typedef worthy::qfamily qFamily_;
        typedef small_bank<size_t> iBank;
        typedef iBank::pointer     iSharedBank;
        typedef small_set<size_t>  iList;
        typedef small_node<size_t> iNode;


        static const char * const fn = "sub_manifold";

        class qFamily : public object, public qFamily_
        {
        public:
            explicit qFamily(const imatrix          &mu,
                             const readable<size_t> &id,
                             const iSharedBank      &io) :
            qFamily_(mu.cols),
            next(0),
            prev(0),
            basis(io),
            ready(io)
            {

                assert(id.size()==mu.rows);
                size_t       i  = mu.rows;
                const size_t ir = id[i];
                if(!grow(mu[ir])) {
                    throw imported::exception(fn,"invalid first sub-space");
                }

                // info
                basis << ir;
                while(--i>0) ready.pre(id[i]);


            }


            //! duplicate all
            inline qFamily(const qFamily &parent) :
            qFamily_(parent),
            next(0),
            prev(0),
            basis(parent.basis),
            ready(parent.ready)
            //ready(parent.ready.io())
            {
            }


            virtual ~qFamily() throw() {}

            inline friend std::ostream & operator<<(std::ostream &os, const qFamily &self)
            {
                const qFamily_ &from = self;
                os << "@" << *self.basis << " : " << from << " +" << *self.ready;
                return os;
            }

            qFamily *next;
            qFamily *prev;
            iList    basis;
            iList    ready;


        private:
            YACK_DISABLE_ASSIGN(qFamily);
        };

        typedef cxx_list_of<qFamily> qBranch;;




        
        void process(bunch<int>    &coeff,
                     const imatrix &mu)
        {
            const size_t m = mu.rows;
            const size_t n = mu.cols;
            assert(coeff.width==n);
            assert(apk::rank_of(mu)==n);

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(m>1);
            assert(n>1);
            cxx_array<size_t>        pool(m);        // indices reservoir
            for(size_t j=1;j<=m;++j) pool[j] = j;    // initial reservoir
            
            iSharedBank io = new iBank();

            //------------------------------------------------------------------
            //
            //
            // trying to suppress each species in turn
            //
            //
            //------------------------------------------------------------------
            for(size_t j=1;j<=m;++j)
            {
                
                //--------------------------------------------------------------
                //
                // prepare reservoir of other index
                //
                //--------------------------------------------------------------
                rolling::down(pool); assert(j==pool[m]);
                
                //--------------------------------------------------------------
                //
                // the species must appear at least twice
                //
                //--------------------------------------------------------------
                if(count_valid(mu[j]) < 2 ) continue;
                std::cerr << std::endl << "nullify species@" << j << " pool=" << pool << " (root=" << pool[m] << ")" << std::endl;


                qBranch parents;
                parents.push_back( new qFamily(mu,pool,io) );
                std::cerr << "\troot=" << *parents.head << std::endl;


                for(size_t cycle=1;parents.size>0;++cycle)
                {
                    std::cerr << "\t@cycle #" << cycle << " with |parents| = " << parents.size << std::endl;
                    qBranch lineage;

                    while(parents.size>0)
                    {

                        auto_ptr<qFamily> parent( parents.pop_front() ); // get the current parent
                        qBranch           children;                      // created children
                        iList             rejected( io );                // index within parent's span

                        assert( (*parent)->size == cycle );
                        assert( parent->ready->size>0);

                        std::cerr << "\t\tparent  = " << parent << std::endl;

                        // loop over next index
                        auto_ptr<qFamily> chld( new qFamily(*parent) );
                        const iNode      *node = parent->ready->head;

                    NEXT_CHILD:
                        const size_t         ir = **node;
                        const readable<int> &cr = mu[ir];
                        if(chld->grow(cr))
                        {
                            chld->basis << ir;
                           // std::cerr << "\t\t|_guess = " << chld << std::endl;

                            children.push_back( chld.yield() );
                            node=node->next;
                            if(node)
                            {
                                chld = new qFamily(*parent);
                                goto NEXT_CHILD;
                            }
                        }
                        else
                        {
                            std::cerr << "bad!" << std::endl;
                            rejected << ir; // in parent's span
                            node=node->next;
                            if(node)
                                goto NEXT_CHILD;
                        }

                        assert(NULL==node);

                        // end of loop: take care of current children

                        if(!children.size)
                        {
                            std::cerr << "End Of Parent!!" << std::endl;
                            exit(0);
                        }

                        // clean up list of ready indices
                        qBranch cleanup;
                        while(children.size)
                        {
                            auto_ptr<qFamily> chld( children.pop_front() );
                            chld->ready -= chld->basis; // used to build child
                            chld->ready -= rejected;    // in familiy span
                            std::cerr << "\t\t|_child = " << chld << std::endl;

                            // check
                            cleanup.push_back( chld.yield() );
                        }

                        lineage.merge_back(cleanup);
                    }

                    parents.swap_with(lineage);


                } // end of cycles for one top-level parent




                std::cerr << "coeff=" << *coeff << std::endl;
                
                
            }
            
            
        }
        
        void nexus:: make_manifold_(cluster &source, const xmlog &xml)
        {
            YACK_XMLSUB(xml,fn);
            YACK_XMLOG(xml,source);
            const size_t n = source.size;

            if(n<=1)
            {
                YACK_XMLOG(xml, "<standalone>");
                return;
            }

            const imatrix mu;
            imatrix       nu(n,M);

            {
                size_t  i=1;
                for(const eq_node *node=source.head;node;node=node->next,++i)
                {
                    const size_t ei = ***node;
                    iota::load(nu[i],Nu[ei]);
                }
            }

            {
                const imatrix nut(nu,transposed);
                select_rows(coerce(mu),nut);
            }


            bunch<int> coeff(n);
            process(coeff,mu);

            std::cerr << "#Coeff=" << coeff->size << std::endl;
            std::cerr << "nu=" << nu << std::endl;
            std::cerr << "mu=" << mu << std::endl;
            
        }
        
        
        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);
            
            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
                make_manifold_(*sharing,xml);
            
        }
        
    }
    
}

