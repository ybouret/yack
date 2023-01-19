
#include "yack/chem/reactor/equilibrium/squad.hpp"

namespace yack {

    namespace chemical {

        squad:: ~squad() throw()
        {
        }

        squad:: squad(const equilibrium &first) :
        object(), eq_repo_(), next(0), prev(0)
        {
            (*this) << first;
        }

        squad:: squad(const squad &other) : object(), eq_repo_(other), next(0), prev(0) {}

        squad * squad:: clone() const {
            return new squad(*this);
        }


        static inline void show(std::ostream &os, const eq_node *node)
        {
            assert(node);
            os << '<' << (***node).name << '>';
        }

        std::ostream &operator<<(std::ostream &os, const squad &self)
        {
            os << "| ";
            const eq_node *node = self.head;
            if(node)
            {
                show(os,node);
                for(node=node->next;node;node=node->next)
                    show(os<<", ",node);
            }
            os << " |";
            return os;
        }

        bool squad:: accepts(const equilibrium  &rhs,
                             const matrix<bool> &detached) const throw()
        {
            assert(size>0);
            const equilibrium &lhs = ***tail;
            const size_t l = *lhs;
            const size_t r = *rhs;
            if(r<=l) {
                return false;
            }
            else
            {
                const readable<bool> &ok = detached[r];
                for(const eq_node *mine=head;mine;mine=mine->next)
                {
                    if( ! ok[ ****mine ] ) return false;
                }
                return true;
            }
        }

        int squad:: compare(const squad *lhs, const squad *rhs) throw()
        {
            switch( __sign::of(lhs->size,rhs->size) )
            {
                case negative: return -1;
                case positive: return  1;
                default:
                    break;
            }

            for(const eq_node *l=lhs->head, *r=rhs->head;l;l=l->next,r=r->next)
            {
                assert(l); assert(r);
                switch( __sign::of(****l,****r) )
                {
                    case negative: return -1;
                    case positive: return  1;
                    case __zero__: continue;
                }

            }

            return 0;
        }


        void squad:: compile()
        {
            for(const eq_node *node=head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                absorb(eq);
            }
        }

    }

}
