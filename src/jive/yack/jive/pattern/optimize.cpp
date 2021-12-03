
#include "yack/jive/pattern/all.hpp"


namespace yack
{
    namespace jive
    {


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


        //! optimize after optimized_logical
        static inline
        pattern * optimized(op_and *p)
        {
            auto_ptr<pattern> q(p);
            if(1==p->size)
            {
                
            }

            return q.yield();
        }

        //! optimize after optimized_logical
        static inline
        pattern * optimized(op_or *p)
        {
            auto_ptr<pattern> q(p);
            p->no_multiple();

            return q.yield();
        }

        //! optimize after optimized_logical
        static inline
        pattern * optimized(op_none *p)
        {
            auto_ptr<pattern> q(p);
            p->no_multiple();

            return q.yield();
        }

        // optime motifs and return corresponding optimized
        template <typename OP>
        static inline pattern *optimized_logical(pattern *p)
        {
            auto_ptr<OP> q(p->as<OP>());
            for(motif *m=q->head;m;m=m->next)
            {
                pattern                *source = & coerce(**m);
                arc_ptr<const pattern>  target = pattern::optimize(source);
                *m = target;
            }
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


