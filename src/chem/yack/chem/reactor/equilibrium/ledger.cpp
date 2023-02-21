#include "yack/chem/reactor/equilibrium/ledger.hpp"

namespace yack {

    namespace chemical {


        void ledger:: grow()
        {
            const eq_repo::ptr tmp = new eq_repo();
            push_back(tmp);
        }

        ledger::  ledger() : ledger_(8,as_capacity) { grow(); }

        ledger:: ~ledger() noexcept {}


        eq_repo & ledger:: degree(const size_t n) {
            while( size() < n )
                grow();
            return *( (*this)[n] );
        }

    }

}
