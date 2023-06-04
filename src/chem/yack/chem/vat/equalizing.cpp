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
                    os << '<' << **node;
                }
            }
            os << ']';
            return os;
        }

        void Equalizing:: tryInsertCursor(const double c, const Actor &a)
        {
            Cursor::Node        *curr = new_node<const double,const Actor,const Species::Fund>(c,a,speciesFund);
            const Extended::Real xi   = **curr;
            try
            {
                switch(size)
                {
                    case 0:
                        push_back(curr);
                        return;

                    case 1:
                        switch(Extended::Comp(xi,(**head)))
                        {
                            case negative: push_front(curr); return;
                            case positive: push_back(curr);  return;
                            case __zero__:
                                zombify(curr);
                                (**head) << *a;
                                break;
                        }

                    default:
                        break;
                }
                throw exception("not implemented");
            }
            catch(...)
            {
                zombify(curr);
                throw;
            }

        }

    }
}
