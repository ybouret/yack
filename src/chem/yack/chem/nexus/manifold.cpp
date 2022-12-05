#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/small/repo.hpp"
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
        
        

        
        void select_rows(imatrix       &w,
                         const imatrix &mu)
        {
            const size_t   m = mu.rows;
            vector<size_t> jrow(m,as_capacity);
            
            // find non proportional rows
            for(size_t j=1;j<=m;++j)
            {
                const readable<int> &curr = mu[j];
                bool                 isOk = true;
                for(size_t k=jrow.size();k>0;--k) {
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
        
        static inline size_t count_valid(const readable<int> &coef) throw()
        {
            size_t count = 0;
            for(size_t i=coef.size();i>0;--i)
            {
                if( 0 != coef[i] ) ++count;
            }
            return count;
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
        

        
        typedef worthy::qfamily    qFamily_;
        typedef small_list<size_t> iList;
        typedef iList::node_type   iNode;
        
        
        class qFamily : public object, public qFamily_
        {
        public:
            explicit qFamily(const size_t dims) :
            qFamily_(dims),
            basis(),
            ready(),
            next(0),
            prev(0)
            {
                
            }
            

            virtual ~qFamily() throw() {}
            
            // copy WITHOUT the ready indices
            qFamily(const qFamily &other) :
            qFamily_(other),
            basis(other.basis),
            ready(),
            next(0),
            prev(0)
            {}

            iList    basis; //!< indices used to construct familiy
            iList    ready; //!< indices ready to grow family

            qFamily  *next; //!< for list
            qFamily  *prev; //!< for list
            
            inline friend std::ostream & operator<<(std::ostream &os, const qFamily &self)
            {
                const qFamily_ &from = self;
                os << "@" << self.basis << "->" << from << "+" << self.ready;
                return os;
            }

#if 0
            inline void ensure(const size_t j)
            {
                for(const iNode *node=indx.head;node;node=node->next)
                {
                    if ( **node == j ) return ;
                }
                indx << j;
            }
#endif

            void to(bunch<int> &coeff)
            {
                for(const worthy::qarray *node=(**this).head;node;node=node->next)
                {
                    const readable<int> &arr = q2i(coeff.work,node->coef);
                    //if(whole_valid(arr))
                    coeff.ensure(arr);
                }
            }
            
            
        private:
            YACK_DISABLE_ASSIGN(qFamily);
            
        };
        
        typedef cxx_list_of<qFamily> qBranch;
        
        
        
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
                std::cerr << std::endl << "nullify species@" << j << " pool=" << pool << std::endl;

                qBranch qRoot;


                //--------------------------------------------------------------
                //
                // initialize root with first row and indices to try
                //
                //--------------------------------------------------------------
                {
                    qFamily *root = qRoot.push_back( new qFamily(n) );
                    if(!root->grow(mu[j])) {
                        throw exception("couldn't start ortho!!!");
                    }
                    root->basis << j;
                    for(size_t i=1;i<m;++i) root->ready << pool[i];
                }

                std::cerr << "qRoot=" << qRoot << std::endl;

                //--------------------------------------------------------------
                //
                //
                //
                //--------------------------------------------------------------
                for(size_t cycle=1;;++cycle)
                {
                    std::cerr << "\t#cycle " << cycle << std::endl;
                    qBranch qHeir;
                    while(qRoot.size)
                    {
                        auto_ptr<qFamily> root( qRoot.pop_front() );
                        std::cerr << "\tprocessing " << root << std::endl;

                        assert((*root)->size<n);
                        assert(root->ready.size>0);
                        assert(root->basis.size+root->ready.size == m);

                        for(const iNode *node=root->ready.head;node;node=node->next)
                        {
                            const size_t         j_r  = **node;
                            const readable<int> &v_r = mu[**node];
                            auto_ptr<qFamily> chld( new qFamily(*root) );

                            if(chld->grow(v_r))
                            {
                                if( (*chld)->size >= n)
                                {
                                    // process and quit
                                    std::cerr << "\tfinal with " << chld << std::endl;

                                }
                                else
                                {
                                    // update and register
                                    chld->basis << j_r;
                                    for(const iNode *scan=root->ready.head;scan;scan=scan->next)
                                    {
                                        if(scan!=node)
                                            chld->ready << **scan;
                                    }
                                    std::cerr << "\tchild with " << chld << std::endl;
                                    qHeir.push_back( chld.yield() );
                                }
                            }

                        }


#if 0
                        // initialize child from root
                        auto_ptr<qFamily> chld( new qFamily(*root) );

                        // try root ready indices to build a new matrix
                        for(const iNode *node=root->ready.head;node;node=node->next)
                        {
                            const size_t jr =**node;

                            if(chld->grow(mu[jr]))
                            {
                                std::cerr << "\tguess with " << chld << std::endl;
                                if( (*chld)->size >= n)
                                {
                                    //------------------------------------------
                                    // last possible child
                                    //------------------------------------------

                                    exit(0);
                                }
                                else
                                {
                                    //------------------------------------------
                                    // update child
                                    //------------------------------------------
                                    chld->basis << jr;
                                    for(const iNode *scan=node->next;scan;scan=scan->next)
                                    {
                                        chld->ready << **scan;
                                    }
                                    assert(chld->basis.size+chld->ready.size == m);
                                    std::cerr << "\tchild with " << chld << std::endl;
                                    qHeir.push_back( chld.yield() );
                                    break;
                                }

                            }
                            else
                            {
                                // take next ready index
                                std::cerr << "\t\tsingular with vector " << mu[jr] << std::endl;
                            }

                        }
#endif

                    }

                    //std::cerr << "heir=" << qHeir << std::endl;


                    break;
                }


#if 0
                continue;

                //--------------------------------------------------------------
                //
                // creating workspace
                //
                //--------------------------------------------------------------
                qBranch  A,B;
                qBranch *qRoot = &A;
                qBranch *qHeir = &B;
                
                //--------------------------------------------------------------
                //
                // initialize root with first row and indices to try
                //
                //--------------------------------------------------------------
                {
                    qFamily *root = qRoot->push_back( new qFamily(n) );
                    if(!root->grow(mu[j])) {
                        throw exception("couldn't start ortho!!!");
                    }
                    for(size_t i=1;i<m;++i) root->indx << pool[i];
                }
                
                std::cerr << "qRoot=" << *qRoot << std::endl;
                
                //--------------------------------------------------------------
                //
                // making levels
                //
                //--------------------------------------------------------------
                for(size_t cycle=1;;++cycle)
                {
                    assert(NULL != qRoot);
                    assert(NULL != qHeir);
                    assert(0==qHeir->size);
                    
                    std::cerr << "-------- at cycle #" << cycle << " --------" << std::endl;
                    
                    // try to grow all the roots with their indices
                    while(qRoot->size)
                    {
                        auto_ptr<qFamily> root( qRoot->pop_front() );
                        std::cerr << "\tprocessing " << *root << std::endl;
                        assert( (*root)->size<n );
                        
                        
                        for(const iNode *node=root->indx.head;node;node=node->next)
                        {
                            const readable<int> &mu_r = mu[**node];
                            std::cerr << "\t\twith " << mu_r << " @" << **node << std::endl;
                            
                            // duplicate without indices
                            auto_ptr<qFamily> chld( new qFamily(*root) );
                            
                            if(!chld->grow(mu_r) || (*chld)->size >= n)
                            {
                                std:: cerr << "\t\t\tdone with " << *chld << std::endl;
                                chld->to(coeff);
                            }
                            else
                            {
                                
                                // append remaining indices to this new child
                                for(const iNode *scan=node->prev;scan;scan=scan->prev)
                                    chld->indx.push_front( new iNode(**scan) );
                                
                                for(const iNode *scan=node->next;scan;scan=scan->next)
                                    chld->indx.push_back( new iNode(**scan) );
                                
                                std::cerr << "\t\t\tchld=" << *chld << std::endl;
                                qHeir->push_back( chld.yield() );
                            }
                            
                        }
                        
                    }
                    
                    assert(qRoot->size<=0);
                    if(qHeir->size<=0)
                        break;
                    cswap(qHeir,qRoot);
                }

#endif
                std::cerr << "coeff=" << *coeff << std::endl;
                
                
            }
            
            
        }
        
        void nexus:: make_manifold_(cluster &source, const xmlog &xml)
        {
            static const char * const fn = "sub_manifold";
            YACK_XMLSUB(xml, "sub_manifold");
            YACK_XMLOG(xml,source);

            if(N<=1)
            {
                YACK_XMLOG(xml, "<standalone>");
                return;
            }

#if 1
            const imatrix Mu;
            {
                const imatrix NuT(Nu,transposed);
                select_rows(coerce(Mu),NuT);
            }
#else
            const imatrix Mu(Nu,transposed);
#endif


            bunch<int> coeff(N);
            process(coeff,Mu);

            std::cerr << "#Coeff=" << coeff->size << std::endl;
            std::cerr << "Nu=" << Nu << std::endl;
            std::cerr << "Mu=" << Mu << std::endl;
            
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

