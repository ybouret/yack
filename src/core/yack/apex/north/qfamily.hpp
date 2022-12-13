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
        typedef small_set<size_t>    qidx_list;
        typedef qidx_list::node_type qidx_node;
        typedef qidx_list::bank_type qidx_bank;
        typedef qidx_list::bank_ptr  qidx_bptr;
        
        class qfamily : public object
        {
        public:
            typedef cxx_list_of<qfamily> list_type;
            static const char clid[];
            virtual ~qfamily() throw();
            
            //!
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

            qfamily(const qfamily &);


            //! one family to one lineage
            template <typename T>
            void generate(list_of<qfamily> &lineage,
                          const matrix<T>  &vbase) const
            {

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
                qidx_list span(basis.io());
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
                }
                
                //--------------------------------------------------------------
                //
                // third pass: remove duplicates...
                //
                //--------------------------------------------------------------

            }

            template <typename T> static inline
            void generate(list_of<qfamily>       &target,
                          const list_of<qfamily> &source,
                          const matrix<T>        &vbase)
            {
                for(const qfamily *origin=source.head;origin;origin=origin->next)
                {
                    list_type lineage;
                    origin->generate(lineage,vbase);
                    target.merge_back(lineage);
                }

            }



            friend std::ostream & operator<<(std::ostream &, const qfamily &);

            clone_ptr<qmatrix> qbase; //!< current qbase
            qidx_list          basis; //!< indices used to form qbase
            qidx_list          ready; //!< indices ready to be used
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

