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

#if 0
        static inline void erase_from(iList &indx, const size_t j) throw()
        {
            iList tmp;
            while(indx.size)
            {
                iNode *node = indx.pop_front();
                if(j==**node)
                {
                    delete node;
                }
                else
                {
                    tmp.push_back(node);
                }
            }
            tmp.swap_with(indx);
        }
#endif

        static inline void erase_from(iList &indx, const iList &ibad) throw()
        {
            iList tmp;
            while(indx.size)
            {
                iNode *node = indx.pop_front();
                if( ibad.whose(**node) ) delete node;
                else  tmp.push_back(node);
            }
            tmp.swap_with(indx);
        }

        static inline void insert_into(iList       &indx,
                                       const iList &inew)
        {
            for(const iNode *node=inew.head;node;node=node->next)
            {
                const size_t j = **node;
                if(!indx.whose(j)) indx << j;
            }
        }

        
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

            // copy without indices
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
                    if(count_valid(arr)>1)
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
                std::cerr << std::endl << "nullify species@" << j << " pool=" << pool << " (but " << pool[m] << ")" << std::endl;



                //--------------------------------------------------------------
                //
                // initialize root with first row and indices to try
                //
                //--------------------------------------------------------------
                qBranch parents;
                {
                    qFamily *root = parents.push_back( new qFamily(n) );
                    if(!root->grow(mu[j])) {
                        throw exception("couldn't start ortho!!!");
                    }
                    root->basis << j;
                    for(size_t i=1;i<m;++i) root->ready << pool[i];
                    std::cerr << " with root=" << *root << std::endl;
                }

                // iterative components
                while(parents.size>0)
                {

                    qBranch lineage;

                    // outer loop
                    while(parents.size>0)
                    {
                        qBranch           narrow;
                        {
                            auto_ptr<qFamily> root( parents.pop_front() );
                            std::cerr << "\troot=" << root << std::endl;

                            bool    inherited = false;
                            iList   reject;
                            qBranch branch;


                            // initialize local branch
                            for(const iNode *node=root->ready.head;node;node=node->next)
                            {
                                auto_ptr<qFamily>    chld( new qFamily(*root) );
                                const size_t         jr = **node;
                                const readable<int> &cr = mu[jr];
                                if(chld->grow(cr)) {
                                    inherited = true;
                                    chld->basis << jr;
                                    if(chld->fully_grown())
                                    {
                                        std::cerr << "\t\t[*] guess: " << chld << std::endl;
                                        chld->to(coeff);
                                    }
                                    else
                                    {
                                        std::cerr << "\t\t[+] guess: " << chld << std::endl;
                                        branch.push_back( chld.yield() );
                                    }
                                }
                                else
                                {
                                    std::cerr << "\t\t[-] rejecting @" << jr << " = " << cr << std::endl;
                                    reject << jr;
                                }
                            }

                            if(!inherited)
                            {
                                std::cerr << "Not Inherited!!!!" << std::endl;
                                exit(0);
                            }



                            while(branch.size)
                            {
                                auto_ptr<qFamily> chld( branch.pop_front() );
                                for(const iNode *node=root->ready.head;node;node=node->next)
                                {
                                    const size_t j = **node;
                                    if(chld->basis.whose(j)) continue; // removed used in basis
                                    if(reject.whose(j))      continue; // remove rejected
                                    chld->ready << j;
                                }
                                std::cerr << "\t\t--> child: " << *chld;

                                qFamily *twin = false;
                                for(qFamily *sibl=narrow.head;sibl;sibl=sibl->next)
                                {
                                    if(*sibl==*chld)
                                    {
                                        twin = sibl;
                                        break;
                                    }
                                }

                                if(twin) {
                                    std::cerr << " twin: " << *twin << std::endl;
                                    for(const iNode *node=chld->basis.head;node;node=node->next)
                                    {
                                        if(twin->grow(mu[**node]))
                                        {
                                            std::cerr << "not consistent" << std::endl;
                                            exit(0);
                                        }
                                    }
                                    erase_from(twin->ready,chld->basis);
                                    if(twin->ready.size<=0)
                                    {
                                        std::cerr << "finished twin!!" << std::endl;
                                        exit(0);
                                    }
                                    else
                                    {
                                        insert_into(twin->basis,chld->basis);
                                        std::cerr << "\t\t+-> child: " << *twin << std::endl;
                                    }

                                }
                                else
                                {
                                    std::cerr << std::endl;
                                    narrow.push_back( chld.yield() );
                                }
                            }

                        }

                        lineage.merge_back(narrow);
                    }

                    assert(parents.size<=0);
                    parents.swap_with(lineage);
                }




                std::cerr << "coeff=" << *coeff << std::endl;
                
                
            }
            
            
        }
        
        void nexus:: make_manifold_(cluster &source, const xmlog &xml)
        {
            static const char * const fn = "sub_manifold";
            YACK_XMLSUB(xml,fn);
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

