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

        static const char * const fn = "sub_manifold";

        typedef worthy::qfamily    qFamily_;
        typedef small_bank<size_t> iBank;
        typedef iBank::pointer     iSharedBank;
        typedef small_set<size_t>  iList;
        typedef small_node<size_t> iNode;
        
        
        //----------------------------------------------------------------------
        //
        // orthogonal family, with
        // indices of used in basis and
        // indices of still not used a.k.a ready :)
        //
        //----------------------------------------------------------------------
        class qFamily : public object, public qFamily_
        {
        public:

            //------------------------------------------------------------------
            //
            //! full setup
            //
            //------------------------------------------------------------------
            explicit qFamily(const readable<size_t> &id,
                             const imatrix          &mu,
                             const iSharedBank      &io) :
            qFamily_(mu.cols),
            next(0),
            prev(0),
            basis(io),
            ready(io)
            {

                // initialize indices
                assert(id.size()==mu.rows); // check sanity
                size_t       i  = mu.rows;  // last index...
                const size_t ir = id[i];    // ...gives major index
                assert(count_valid(mu[ir])>1);

                // build first vector
                if(!grow(mu[ir]))
                    throw imported::exception(fn,"invalid first sub-space");

                
                // load initial info
                basis << ir;                     // store major row
                while(--i>0) ready.pre(id[i]);   // store remaining

            }
            
            //------------------------------------------------------------------
            //
            //! duplicate all
            //
            //------------------------------------------------------------------
            inline qFamily(const qFamily &parent) :
            object(),
            qFamily_(parent),
            next(0),
            prev(0),
            basis(parent.basis),
            ready(parent.ready)
            {
            }
            
            
            virtual ~qFamily() throw() {}
            
            inline friend std::ostream & operator<<(std::ostream &os, const qFamily &self)
            {
                const qFamily_ &from = self;
                os << "@" << *self.basis << " : " << from << " +" << *self.ready;
                return os;
            }
            
            qFamily *next;  //!< for list
            qFamily *prev;  //!< for list
            iList    basis; //!< used to build basis
            iList    ready; //!< candidate
            
            
            inline void to(bunch<int> &coef) const
            {
                const qFamily_ &self = *this;
                assert(self->size>1);
                assert(self->head);
                assert(self->head->next);

                std::cerr << "\t |_parse = " << self << std::endl;

                for(const worthy::qarray *arr=self->head->next;arr;arr=arr->next)
                {
                    coef.ensure( q2i(coef.work,arr->coef) );
                }
            }
            
        private:
            YACK_DISABLE_ASSIGN(qFamily);
        };

        //----------------------------------------------------------------------
        // list of orthogonal families
        //----------------------------------------------------------------------
        typedef cxx_list_of<qFamily> qBranch;
        

        // create next generation
        static inline
        void create_next_gen(qBranch       &target,
                             const qFamily &source,
                             const imatrix &mu,
                             iSharedBank   &io)
        {
            assert(worthy::in_progress==source.situation);

            iList             span(io);
            auto_ptr<qFamily> chld( new qFamily(source) );
            const iNode      *node = source.ready->head;
            
        NEXT_CHILD:
            const size_t         ir = **node;
            const readable<int> &cr = mu[ir];
            if(chld->grow(cr))
            {
                //--------------------------------------------------------------
                //
                // valid index!
                //
                //--------------------------------------------------------------
                chld->basis << ir;                // register in basis
                //std::cerr << "\t\t|_guess = " << chld << std::endl;
                target.push_back( chld.yield() ); // register as possible child
                node=node->next;
                if(node)
                {
                    chld = new qFamily(source);
                    goto NEXT_CHILD;
                }
            }
            else
            {
                //--------------------------------------------------------------
                //
                // invalid index :
                // in parent's span for the rest of the cycles
                //
                //--------------------------------------------------------------
                span << ir;
                node=node->next;
                if(node)
                    goto NEXT_CHILD;
            }
            assert(NULL==node);

            // first pass: cleaning up indices
            for(qFamily *member=target.head;member;member=member->next)
            {
                member->ready -= member->basis;
                member->ready -= span;
                std::cerr << "\t|_child  = " << *member << std::endl;
            }
        }


        static inline
        void complete(qFamily &source, const imatrix &mu)
        {
            assert(worthy::almost_done==source.situation);
            assert(source.ready->size>0);
            for(const iNode *node=source.ready->head;node;node=node->next)
            {
                if(source.grow(mu[**node])) return;
            }
            throw imported::exception(fn,"unable to complete family");
        }

        static inline
        void process_one_species(bunch<int>             &coef,
                                 const readable<size_t> &jndx,
                                 const imatrix          &mu,
                                 iSharedBank            &io)
        {

            //------------------------------------------------------------------
            //
            // initialize top-level source
            //
            //------------------------------------------------------------------
            qBranch      genitors;
            genitors.push_back( new qFamily(jndx,mu,io) );
            std::cerr << "\tgenitors = " << *genitors.head << std::endl;

            //------------------------------------------------------------------
            //
            // iterative cycles from genitors
            //
            //------------------------------------------------------------------
            for(size_t cycle=1;genitors.size;++cycle)
            {
                qBranch children;

                //--------------------------------------------------------------
                //
                // process all current genitors
                //
                //--------------------------------------------------------------
                while(genitors.size)
                {
                    //----------------------------------------------------------
                    //
                    // extract next genitor => source
                    //
                    //----------------------------------------------------------
                    auto_ptr<qFamily> source( genitors.pop_front() ); // get the current parent
                    assert( (*source)->size == cycle );               // sanity check
                    assert( source->ready->size>0    );               // sanity check

                    switch(source->situation)
                    {
                        case worthy::fully_grown:
                            throw imported::exception(fn,"fully grown family shouldn't happen here");


                        case worthy::almost_done:
                            // all children will produce the same last vector
                            // so we take the first that matches by completing
                            complete(*source,mu); assert(worthy::fully_grown==source->situation);
                            std::cerr << "\t|_child1 = " << source << std::endl;

                            // process and discard this source
                            source->to(coef);
                            continue;

                        case worthy::in_progress:
                            break;
                    }

                    //----------------------------------------------------------
                    //
                    // source is still in progress => new generation
                    //
                    //----------------------------------------------------------
                    qBranch target;     // new generation
                    create_next_gen(target,*source,mu,io);

                    children.merge_back(target);
                }

                children.swap_with(genitors);
            }


            
        }
        
        
        static inline
        void process_all_species(bunch<int>    &coef,
                                 const imatrix &mu)
        {
            const size_t m = mu.rows;
            assert(mu.cols>1);
            assert(mu.rows>1);
            assert(coef.width==mu.cols);
            assert(apk::rank_of(mu)==mu.cols);
            
            //------------------------------------------------------------------
            //
            //
            // initialize memory
            //
            //
            //------------------------------------------------------------------
            assert(m>1);
            iSharedBank              io = new iBank(); // I/O for indices
            cxx_array<size_t>        jndx(m);          // indices reservoir
            for(size_t j=1;j<=m;++j) jndx[j] = j;      // initial reservoir
            
            
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
                // prepare pool of indices, last one will be the leading index
                //
                //--------------------------------------------------------------
                rolling::down(jndx); assert(j==jndx[m]);
                
                //--------------------------------------------------------------
                //
                // the species must appear at least twice
                //
                //--------------------------------------------------------------
                if(count_valid(mu[j]) < 2 ) continue;
                std::cerr << std::endl << "nullify species@" << j << " jndx=" << jndx << " (root=" << jndx[m] << ")" << std::endl;
                
                //--------------------------------------------------------------
                //
                // process the species
                //
                //--------------------------------------------------------------
                process_one_species(coef,jndx,mu,io);
            }
            
            
            std::cerr << "coef=" << *coef << std::endl;
            
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
            
            std::cerr << "nu=" << nu << std::endl;
            std::cerr << "mu=" << mu << std::endl;

            bunch<int> coeff(n);
            process_all_species(coeff,mu);
            
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

