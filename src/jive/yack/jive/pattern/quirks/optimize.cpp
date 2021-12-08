
#include "yack/jive/pattern/all.hpp"


namespace yack
{
    namespace jive
    {

        //----------------------------------------------------------------------
        //
        // reduce small range
        //
        //----------------------------------------------------------------------
        static inline pattern *optimized( within *p )
        {
            auto_ptr<pattern> guard(p);

            if(p->lower>=p->upper)
            {
                return new single(p->lower);
            }
            else
            {
                return guard.yield();
            }
        }


        //----------------------------------------------------------------------
        //
        // optimize after optimized_logical
        //
        //----------------------------------------------------------------------
        static inline
        pattern * optimized(op_and *p)
        {
            auto_ptr<pattern> q(p);
            if(1==p->size)
            {
                // reduce complexity
                return p->pop_back();
            }
            else
            {
                // nothing to do
                return q.yield();
            }
        }

        //----------------------------------------------------------------------
        //
        // optimize after optimized_logical
        //
        //----------------------------------------------------------------------
        static inline
        pattern * optimized(op_or *p)
        {
            auto_ptr<pattern> q(p);
            p->rewrite();     // reduce complexity
            if(1==p->size)
            {
                // reduce complexity
                return p->pop_back();
            }
            else
            {
                // nothing to do
                return q.yield();
            }
        }

        //----------------------------------------------------------------------
        //
        // optimize after optimized_logical
        //
        //----------------------------------------------------------------------
        static inline
        pattern * optimized(op_none *p)
        {
            auto_ptr<pattern> q(p);
            p->rewrite();    // reduce complexity
            if(1==p->size)
            {
                pattern *m = p->head;
                if(m->is<single>())
                {
                    return new except(m->as<single>()->code);
                }
            }

            return q.yield();
        }

        //----------------------------------------------------------------------
        //
        // optimize operands and return corresponding typed optimized
        //
        //----------------------------------------------------------------------
        template <typename OP>
        static inline pattern *optimized_logical(pattern *p)
        {
            auto_ptr<OP> q(p->as<OP>());

            // recursive optimization
            {
                patterns ops;
                while(q->size)
                {
                    pattern *r = pattern::optimize(q->pop_front());
                    if(OP::mark==r->uuid)
                    {
                        // same kind promotion
                        ops.merge_back( *(r->as<OP>()) );
                        delete r;
                    }
                    else
                    {
                        ops.push_back(r);
                    }
                }
                q->swap_with(ops);
            }

            // send to proper function
            return optimized( q.yield() );
        }



        pattern * pattern::optimize(pattern *p)
        {
            assert(p);
            switch(p->uuid)
            {
                    //----------------------------------------------------------
                    //
                    // basic
                    //
                    //----------------------------------------------------------
                case within::mark: return optimized( p->as<within>() );

                    //----------------------------------------------------------
                    //
                    // logical
                    //
                    //----------------------------------------------------------
                case op_and::  mark: return optimized_logical<op_and>(p);
                case op_or::   mark: return optimized_logical<op_or>(p);
                case op_none:: mark: return optimized_logical<op_none>(p);

                default:
                    break;
            }
            return p;

        }

    }

}


