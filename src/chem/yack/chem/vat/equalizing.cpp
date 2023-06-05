#include "yack/chem/vat/equalizing.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace Chemical
    {
        Equalizing:: Equalizing(const Species::Fund &s,
                                const Cursor::Fund  &f) noexcept :
        Cursor::List(f),
        speciesFund(s)
        {
        }

        Equalizing:: ~Equalizing() noexcept
        {
        }



        std::ostream & operator<<(std::ostream &os, const Equalizing &self)
        {
            os << '[';
            const Cursor::Node *node = self.head;
            if(node)
            {
                os << **node;
                for(node=node->next;node;node=node->next)
                {
                    os << " < " << **node;
                }
            }
            os << ']';
            return os;
        }

        bool Equalizing:: isValid() const noexcept
        {
            if(size>=2)
            {
                const Cursor::Node *node = head;         assert(node);
                const Cursor::Node *next = node->next;   assert(next);
                while(next)
                {
                    if(**node>=**next) return false;
                    node = next;
                    next = next->next;
                }
            }
            return true;
        }

        void Equalizing:: tryInsertCursor(const double c, const Actor &a)
        {
            Cursor::Node        *curr = new_node<const double,const Actor,const Species::Fund>(c,a,speciesFund);
            const Extended::Real xi   = **curr;
            std::cerr << "==> adding " << *curr << " to " << *this << std::endl;
            try
            {
                switch(size)
                {
                        //------------------------------------------------------
                        // first insertion
                        //------------------------------------------------------
                    case 0:
                        push_back(curr); assert(isValid());
                        return;

                        //------------------------------------------------------
                        // simplified insertion
                        //------------------------------------------------------
                    case 1:
                        switch(Extended::Comp(xi,**head))
                        {
                            case negative: push_front(curr);              assert(isValid()); return;
                            case positive: push_back(curr);               assert(isValid()); return;
                            case __zero__: zombify(curr); (**head) << *a; assert(isValid()); return;
                        }
                        return;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                //
                // generic insertion
                //
                //--------------------------------------------------------------
                assert(size>=2);

                //--------------------------------------------------------------
                // check first value
                //--------------------------------------------------------------
                Cursor::Node *lo = head;
                switch( Extended::Comp(xi,**lo) )
                {
                    case negative: push_front(curr);              assert(isValid()); return;
                    case __zero__: zombify(curr); (**head) << *a; assert(isValid()); return;
                    case positive:
                        break;
                }

                //--------------------------------------------------------------
                // check last value
                //--------------------------------------------------------------
                const Cursor::Node * const up = tail;
                switch( Extended::Comp(xi,**up) )
                {
                    case negative: break;
                    case __zero__: zombify(curr); (**head) << *a; assert(isValid()); return;
                    case positive: push_back(curr);               assert(isValid()); return;
                }

                //--------------------------------------------------------------
                // find bracketing nodes
                //--------------------------------------------------------------
                Cursor::Node *nx = lo->next;
                while(nx!=up)
                {
                    switch( Extended::Comp(xi,**nx) )
                    {
                        case negative: goto INSERT;
                        case __zero__: zombify(curr); (**head) << *a; assert(isValid()); return;
                        case positive:
                            break;
                    }
                    lo=nx;
                    nx=nx->next;
                }

                assert(up==nx);

            INSERT:
                insert_after(lo,curr);
                assert(isValid());
            }
            catch(...)
            {
                zombify(curr);
                throw;
            }

        }


        void Equalizing:: findBestEffort(Limit &regulating)
        {
            assert(regulating.size>0);
            assert(this->size>0);
            assert(this->isValid());

            //------------------------------------------------------------------
            //
            // test against head
            //
            //------------------------------------------------------------------
            const Cursor::Node *curr = head;
            switch( Extended::Comp(regulating,**curr) )
            {
                case negative: return;                                     // limited by regulating
                case __zero__: regulating.merge_back_copy(**curr); return; // same values
                case positive:
                    if(1==size)
                    {
                        // specific most favorable case
                        regulating = **curr;
                        return;
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            // test against tail
            //
            //------------------------------------------------------------------
            assert(size>=2);
            const Cursor::Node * const last = tail; assert(last!=curr);
            switch( Extended::Comp(regulating,**last) )
            {
                case positive: regulating = **last;                return; // most favorable case!
                case __zero__: regulating.merge_back_copy(**last); return; // most favorable case as well
                case negative: break;
            }

            //------------------------------------------------------------------
            //
            // generic sequential search
            //
            //------------------------------------------------------------------
            const Cursor::Node *       next = curr->next;
            while(next!=last)
            {
                switch( Extended::Comp(regulating,**next))
                {
                    case negative: goto FOUND_GENERIC;
                    case __zero__: regulating.merge_back_copy(**next); return;
                    case positive: break;
                }
                curr = next;
                next = next->next;
            }
            assert(next==last);

        FOUND_GENERIC:
            assert( regulating > **curr );
            assert( regulating < **next );
            regulating = **curr;
        }

    }
}
