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
        
        void select_clan(sp_list       &clan,
                         imatrix       &work,
                         const imatrix &mu,
                         const library &lib)
        {
            //------------------------------------------------------------------
            //
            // initialize indices
            //
            //------------------------------------------------------------------
            assert(mu.rows==lib.size());
            assert(0==clan.size);
            const size_t   m = mu.rows;
            vector<size_t> jrow(m,as_capacity);

            //------------------------------------------------------------------
            //
            // find not zero and not propto previously selected
            //
            //------------------------------------------------------------------
            for(const snode *sn=lib.head();sn;sn=sn->next)
            {
                const species &s = ***sn;
                const size_t   j = *s;
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
                {
                    jrow << j;
                    clan << &s;
                }
            }

            //------------------------------------------------------------------
            //
            // build sub-matrix
            //
            //------------------------------------------------------------------
            const size_t dims = jrow.size(); assert(dims==clan.size);
            if(dims<=0) throw imported::exception("select sub-manifold species","unexpected zero match!!");

            work.make(dims,mu.cols);
            for(size_t k=dims;k>0;--k)
                iota::load(work[k],mu[ jrow[k] ]);

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

                //--------------------------------------------------------------
                //
                // initialize indices
                //
                //--------------------------------------------------------------
                assert(id.size()==mu.rows); // check sanity
                size_t       i  = mu.rows;  // last index...
                const size_t ip = id[i];    // ...gives major index
                const readable<int> &primary = mu[ip];
                assert(count_valid(primary)>1);

                //--------------------------------------------------------------
                //
                // build first vector
                //
                //--------------------------------------------------------------
                if(!grow(primary))
                    throw imported::exception(fn,"invalid primary sub-space");


                //--------------------------------------------------------------
                //
                // load primary info
                //
                //--------------------------------------------------------------
                basis << ip;

                //--------------------------------------------------------------
                //
                // load other
                //
                //--------------------------------------------------------------
                while(--i>0)
                {
                    const size_t          ir      = id[i];
                    //const readable<int> & replica = mu[ id[i] ];
                    ready.pre(ir);   // store remaining
                }

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
                std::cerr << from << " / " << *self.basis << "+" << *self.ready;
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

                //std::cerr << "\t |_parse = " << self << std::endl;
                for(const worthy::qarray *arr=self->head->next;arr;arr=arr->next)
                {
                    const readable<int> &host = q2i(coef.work,arr->coef);
                    if(count_valid(host)<2) continue;
                    coef.ensure(host);
                }
            }
            
        private:
            YACK_DISABLE_ASSIGN(qFamily);
        };

        //----------------------------------------------------------------------
        // list of orthogonal families
        //----------------------------------------------------------------------
        typedef cxx_list_of<qFamily> qBranch;

        //----------------------------------------------------------------------
        //
        // reduce complexity
        //
        //----------------------------------------------------------------------
        enum consolidate_type
        {
            consolidate_fast,
            consolidate_full
        };

        // find by comparing only last coefficients
        static inline
        qFamily *find_fast_twin_of(const qFamily &chld,
                                   const qBranch &target)
        {

            const qFamily_      &rhs = chld; assert(rhs->tail);
            const readable<apq> &R   = rhs->tail->coef;
            for(qFamily *scan=target.head;scan;scan=scan->next)
            {
                const qFamily_ &lhs = *scan;

                assert( lhs->size == rhs->size );
                assert( lhs->tail != NULL);

#ifndef NDEBUG
                for(const worthy::qarray *l=lhs->head, *r=rhs->head;l!=lhs->tail;r=r->next,l=l->next)
                {
                    assert( l->coef == r->coef );
                }
#endif

                if( lhs->tail->coef == R )
                {
                    return scan;
                }
            }
            return NULL;
        }

        //find by comparing full family
        static inline
        qFamily *find_full_twin_of(const qFamily &chld,
                                   const qBranch &target)
        {
            for(qFamily *scan=target.head;scan;scan=scan->next)
            {
                assert((*scan)->size == chld->size);
                if( *scan == chld )
                {
                    return scan;
                }
            }
            return NULL;
        }

        static inline
        void consolidate(qBranch                &source,
                         const imatrix          &mu,
                         const consolidate_type  ct)
        {
            const size_t oldCount = source.size;
            if(oldCount<=0) return;
            qBranch      target;
            while(source.size)
            {
                auto_ptr<qFamily> chld = source.pop_front();
                qFamily          *twin = NULL;
                switch(ct)
                {
                    case consolidate_full: twin = find_full_twin_of(*chld,target); break;
                    case consolidate_fast: twin = find_fast_twin_of(*chld,target); break;
                }


                if(twin)
                {
                    // merge info into twin
                    twin->basis += chld->basis;
                    twin->ready += chld->ready;
                    twin->ready -= twin->basis;
                    //std::cerr << "\t|_twin   = " << *twin << std::endl;
                    for(const iNode *node=twin->basis->head;node;node=node->next)
                    {
                        assert(!twin->grow(mu[**node]));
                    }
                }
                else
                {
                    target.push_back( chld.yield());
                }
            }
            target.swap_with(source);

#if 0
            const size_t newCount = source.size;

            std::cerr << "\t--> consolidated ";
            switch(ct)
            {
                case consolidate_full: std::cerr << "FULL"; break;
                case consolidate_fast: std::cerr << "FAST"; break;
            }
            std::cerr << " : " << oldCount << " -> " << newCount << std::endl;
#endif
            
        }


        //----------------------------------------------------------------------
        //
        // create next generation
        //
        //----------------------------------------------------------------------
        static inline
        void create_next_gen(qBranch       &target,
                             const qFamily &source,
                             const imatrix &mu,
                             iSharedBank   &io)
        {
            assert(worthy::in_progress==source.situation);
            assert(0==target.size);

            iList             span(io);

            {
                auto_ptr<qFamily> chld( new qFamily(source) );
                const iNode      *node = source.ready->head;

            NEXT_CHILD:
                const size_t         ir = **node;
                const readable<int> &cr = mu[ir];
                if(chld->grow(cr))
                {
                    //----------------------------------------------------------
                    //
                    // valid index!
                    //
                    //----------------------------------------------------------
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
                    //----------------------------------------------------------
                    //
                    // invalid index :
                    // in parent's span for the rest of the cycles
                    //
                    //----------------------------------------------------------
                    span << ir;
                    node=node->next;
                    if(node)
                        goto NEXT_CHILD;
                }
                assert(NULL==node);
            }


            //----------------------------------------------------------
            //
            // first pass: cleaning up indices
            //
            //----------------------------------------------------------
            for(qFamily *member=target.head;member;member=member->next)
            {
                member->ready -= member->basis;
                member->ready -= span;
            }

            consolidate(target,mu,consolidate_fast);


            // verbose
            for(qFamily *member=target.head;member;member=member->next)
            {
                //std::cerr << "\t|_child  = " << *member << std::endl;
                assert(member->ready.excludes(span));
            }

        }


        static inline
        void complete_family(qFamily       &source,
                             const imatrix &mu)
        {
            assert(worthy::almost_done==source.situation);
            assert(source.ready->size>0);
            for(const iNode *node=source.ready->head;node;node=node->next)
            {
                if(source.grow(mu[**node])) return;
            }
            throw imported::exception(fn,"unable to complete family");
        }


        void update_all_combinations(bunch<int>             &coef,
                                     qBranch                &genitors,
                                     const imatrix          &mu,
                                     iSharedBank            &io,
                                     const xmlog            &xml)

        {
            static const char * const here = "nexus::update_all_combinations";

            qBranch children;

            //------------------------------------------------------------------
            //
            // make a new generation for each present genitor
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- |genitors| = " << genitors.size );
            while(genitors.size)
            {
                auto_ptr<qFamily> source = genitors.pop_front();
                assert(source->ready->size>0);
                switch(source->situation)
                {
                    case worthy::fully_grown:
                        YACK_XMLOG(xml, "[!] " << source);
                        throw imported::exception(here,"unexpected fully grown family!");

                    case worthy::almost_done:
                        // all children will produce the same last vector
                        // so we take the first that matches by completing
                        complete_family(*source,mu); assert(worthy::fully_grown==source->situation);
                        YACK_XMLOG(xml, "[*] " << source);

                        // process and discard this source
                        source->to(coef);
                        continue;

                    case worthy::in_progress:
                        YACK_XMLOG(xml, "[+] " << source);
                        qBranch target;                         // local new generation
                        create_next_gen(target,*source,mu,io);  // create it
                        children.merge_back(target);            // assemble in children
                        break;
                }
            }
            assert(0==genitors.size);

            //------------------------------------------------------------------
            //
            // remove all duplicates, full span
            //
            //------------------------------------------------------------------
            consolidate(children,mu,consolidate_full);

            //------------------------------------------------------------------
            //
            // replace genitors content
            //
            //------------------------------------------------------------------
            while(children.size)
            {
                auto_ptr<qFamily> chld = children.pop_front();
                genitors.push_back(chld.yield());
            }

        }



        static inline
        void create_all_combinations(bunch<int>    &coef,
                                       const sp_list &sl,
                                       const imatrix &mu,
                                       const xmlog   &xml,
                                       const library &lib)
        {
            static const char * const here = "create_all_combinations";
            YACK_XMLSUB(xml,here);

            assert(mu.cols>1);
            assert(mu.rows>1);
            assert(sl.size==mu.rows);
            assert(coef.width       == mu.cols);
            assert(apk::rank_of(mu) == mu.cols);


            //------------------------------------------------------------------
            //
            //
            // initialize memory
            //
            //
            //------------------------------------------------------------------
            const size_t             m  = mu.rows;
            iSharedBank              io = new iBank();   // I/O for indices
            cxx_array<size_t>        jndx(m);            // indices reservoir
            qBranch                  genitors;           // top-level genitors
            jndx.ld_incr<size_t>(1); assert(m==jndx[m]); // initial indices

            //------------------------------------------------------------------
            //
            //
            // loading all genitors for vanishing species
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- loading all genitors");
            for(const sp_node *sn=sl.head;sn;sn=sn->next)
            {

                //--------------------------------------------------------------
                //
                // prepare pool of indices, last one will be the leading index
                //
                //--------------------------------------------------------------
                rolling::down(jndx);
                const species &s = **sn;
                const size_t   j = jndx[m];

                //--------------------------------------------------------------
                //
                // the species must appear at least twice
                //
                //--------------------------------------------------------------
                if(count_valid(mu[j]) < 2 ) continue;

                //--------------------------------------------------------------
                //
                // initialize a genitor
                //
                //--------------------------------------------------------------
                genitors.push_back( new qFamily(jndx,mu,io) );
                if(xml.verbose)
                {
                    lib.pad(*xml << "[" << s.name << "]",s) << " with "<< *(genitors.tail) << std::endl;
                }
            }

            for(const qFamily *lhs=genitors.head;lhs;lhs=lhs->next)
            {
                for(const qFamily *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    if( *lhs == *rhs ) throw imported::exception(here,"multiple genitors!");
                }
            }

            //------------------------------------------------------------------
            //
            //
            // upgrade all
            //
            //
            //------------------------------------------------------------------
            while(genitors.size)
            {
                update_all_combinations(coef,genitors,mu,io,xml);
            }
        }
        
        
        
        void nexus:: make_manifold_(cluster     &cls,
                                    const xmlog &xml)
        {
            YACK_XMLSUB(xml,fn);
            YACK_XMLOG(xml,cls);

            //------------------------------------------------------------------
            //
            // check local size
            //
            //------------------------------------------------------------------
            const size_t n = cls.size;
            
            if(n<=1) {
                YACK_XMLOG(xml, "<standalone>");
                return;
            }
            

            //------------------------------------------------------------------
            //
            // select local topology
            //
            //------------------------------------------------------------------
            const imatrix mu;
            const sp_list sl;
            {
                //--------------------------------------------------------------
                //
                // build local topology
                //
                //--------------------------------------------------------------
                imatrix       nu(n,M);
                {
                    size_t  i=1;
                    for(const eq_node *node=cls.head;node;node=node->next,++i)
                        iota::load(nu[i],Nu[***node]);
                }

                //--------------------------------------------------------------
                //
                // select local vectors: the problem becomes abstract
                //
                //--------------------------------------------------------------
                {
                    const imatrix nut(nu,transposed);
                    select_clan(coerce(sl),coerce(mu),nut,corelib);
                }

                if(verbose)
                {
                    YACK_XMLOG(xml,"-- local topogy");
                    std::cerr << "\tnu=" << nu << std::endl;
                    std::cerr << "\tmu=" << mu << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            // build all possible weights
            //
            //------------------------------------------------------------------
            bunch<int> coef(n);
            create_all_combinations(coef,sl,mu,xml,corelib);

#if 0
            std::cerr << "Listing: #" << coef->size << std::endl;
            for(const bunch<int>::entry *ep=coef->head;ep;ep=ep->next)
            {
                std::cerr << " --> " << *ep << std::endl;
            }
            return;
#endif

            //------------------------------------------------------------------
            //
            // expanding
            //
            //------------------------------------------------------------------
            cluster        repo;
            {
                cxx_array<int> gcof(N);
                for(const bunch<int>::entry *ep=coef->head;ep;ep=ep->next)
                {
                    const readable<int> &lcof = *ep;
                    gcof.ld(0);
                    size_t  i=1;
                    for(const eq_node *node=cls.head;node;node=node->next,++i)
                    {
                        const size_t ei = ***node;
                        gcof[ei] = lcof[i];
                    }
                    //YACK_XMLOG(xml, "--> " << gcof);
                    repo << &promote_mixed(gcof);
                }
            }

            YACK_XMLOG(xml,"-- added #" << repo.size << " to local cluster");
            cls.merge_back(repo);

        }
        
        
        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);
            
            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
                make_manifold_(*sharing,xml);

            //std::cerr << lattice << std::endl;
            std::cerr << singles.size() << " -> " << lattice.size() << std::endl;
        }
        
    }
    
}

