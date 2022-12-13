//! \file

#ifndef YACK_NORTH_QFAMILY_INCLUDED
#define YACK_NORTH_QFAMILY_INCLUDED 1

#include "yack/apex/north/qmatrix.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/data/small/set.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace north
    {
        //______________________________________________________________________
        //
        //
        // types to manage set of indices
        //
        //______________________________________________________________________
        typedef small_set<size_t>   qidx_set;  //!< alias
        typedef qidx_set::node_type qidx_node; //!< alias
        typedef qidx_set::bank_type qidx_bank; //!< alias
        typedef qidx_set::bank_ptr  qidx_bptr; //!< alias


        //______________________________________________________________________
        //
        //
        //! family of vectors forming an orthogonal matrix
        //
        //______________________________________________________________________
        class qfamily : public object
        {
        public:
            typedef cxx_list_of<qfamily> list_type; //!< alias
            static const char            clid[];    //!< "qfamily"

            virtual ~qfamily() throw(); //!< cleanup
            
            //! create a new family
            /**
             \param rindx rows index, last is active (for use with roll)
             \param vbase base of vectors to use
             \param idxIO shared cache for indices
             */
            template <typename T>
            inline   qfamily(const readable<size_t> &rindx,
                             const matrix<T>        &vbase,
                             const qidx_bptr        &idxIO) :
            qbase( new qmatrix(vbase.cols) ),
            basis(idxIO),
            ready(idxIO),
            next(NULL),
            prev(NULL)
            {
                const size_t nr = rindx.size(); assert(nr>0);
                const size_t ir = rindx[nr];    assert(ir<=vbase.rows);
                if(!qbase->grow(vbase[ir])) throw_invalid_init(ir);
                assign_all_indices(rindx);
            }

            //! hard copy
            qfamily(const qfamily &);


            const qmatrix & operator*() const throw() { return *qbase; }

            //! generate lineage
            template <typename T>
            void generate(list_of<qfamily> &lineage,
                          const matrix<T>  &vbase) const
            {

                std::cerr << "generate from " << qbase << std::endl;
                //--------------------------------------------------------------
                //
                // check situation
                //
                //--------------------------------------------------------------
                switch( qbase->situation )
                {
                    case fully_grown: assert(ready->size<=0);      return; // no child
                    case almost_done: try_complete(lineage,vbase); return; // at most one child
                    case in_progress: break;                               // possible multiple children
                }

                //--------------------------------------------------------------
                //
                // fist pass: create children and detect indices in span
                //
                //--------------------------------------------------------------
                qidx_set span(basis.io());
                {
                    auto_ptr<qfamily> chld =  new qfamily(*this);
                    const qidx_node  *node =  ready->head;

                NEXT_CHILD:
                    const size_t         ir = **node;
                    const readable<T>   &cr = vbase[ir];
                    if(chld->qbase->grow(cr))
                    {
                        //------------------------------------------------------
                        //
                        // valid index!
                        //
                        //------------------------------------------------------
                        chld->basis << ir;                 // register in basis
                        lineage.push_back( chld.yield() ); // register as possible child

                        // prepare next node
                        node=node->next;
                        if(node)
                        {
                            // prepare next child
                            chld =  new qfamily(*this);
                            goto NEXT_CHILD;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // invalid index :
                        // in parent's span for the rest of the cycles
                        //
                        //------------------------------------------------------
                        span << ir;
                        node=node->next;
                        if(node)
                            goto NEXT_CHILD;
                    }
                    assert(NULL==node);
                }

                //--------------------------------------------------------------
                //
                // second pass: cleanup ready for each member of the lineage
                //
                //--------------------------------------------------------------
                for(qfamily *member=lineage.head;member;member=member->next)
                {
                    member->ready -= member->basis;
                    member->ready -= span;
                    std::cerr << "\t-> " << *member << std::endl;

                }
                
                //--------------------------------------------------------------
                //
                // third pass: remove duplicates...
                //
                //--------------------------------------------------------------
                reduce_freshly_created(lineage);
                for(const qfamily *f=lineage.head;f;f=f->next)
                {
                    std::cerr << "\t-> " << *f << std::endl;
                }
            }


            //! generate full lineage of source
            template <typename T> static inline
            void generate(list_of<qfamily>       &target,
                          const list_of<qfamily> &source,
                          const matrix<T>        &vbase)
            {
                assert(target.size<=0);
                for(const qfamily *origin=source.head;origin;origin=origin->next)
                {
                    list_type lineage;
                    origin->generate(lineage,vbase);
                    fusion_already_reduced(target,lineage);
                }

            }


            static void reduce_freshly_created(list_of<qfamily> &lineage);                           //!< reduce fresh lineage
            static void fusion_already_reduced(list_of<qfamily> &target, list_of<qfamily> &source);  //! merge/reduce two reduced lineages
            

            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            clone_ptr<qmatrix> qbase; //!< current qbase
            qidx_set           basis; //!< indices used to form qbase
            qidx_set           ready; //!< indices ready to be used
            qfamily           *next;  //!< for list
            qfamily           *prev;  //!< for list


            
        private:
            YACK_DISABLE_ASSIGN(qfamily);
            static void throw_invalid_init(const size_t ir);
            void        assign_all_indices(const readable<size_t> &rindx);


            template <typename T>
            void try_complete(list_of<qfamily> &lineage,
                              const matrix<T>  &vbase) const
            {
                assert(almost_done==qbase->situation);
                if(ready->size)
                {
                    auto_ptr<qfamily> child = new qfamily(*this);
                    bool              found = false;
                    while(child->ready->size)
                    {
                        const size_t i = child->ready.pull_front();
                        assert(i>0); assert(i<=vbase.rows); assert(!child->basis.contains(i));
                        child->basis.add(i);
                        if(child->qbase->grow(vbase[i])) {
                            found = true;
                            break;
                        }
                    }
                    while(child->ready->size){
                        const size_t i = child->ready.pull_front();
                        assert(!child->basis.contains(i));
                        child->basis.add(i);
                    }
                    if(found) lineage.push_back( child.yield() );
                }
                else
                {
                    // nothing else to do
                    return;
                }
            }


        };
        
    }
}

#endif

