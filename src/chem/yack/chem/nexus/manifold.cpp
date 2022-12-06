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

        class qFamily : public object, public qFamily_
        {
        public:
            explicit qFamily(const size_t dims) : qFamily_(dims)
            {
            }

            virtual ~qFamily() throw() {}

            qFamily *next;
            qFamily *prev;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qFamily);
        };



        
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






                std::cerr << "coeff=" << *coeff << std::endl;
                
                
            }
            
            
        }
        
        void nexus:: make_manifold_(cluster &source, const xmlog &xml)
        {
            static const char * const fn = "sub_manifold";
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

