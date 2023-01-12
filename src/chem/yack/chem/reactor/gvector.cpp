
#include "yack/chem/reactor/gvector.hpp"

namespace yack {

    namespace chemical {


        void gvector:: grow()
        {
            const eq_repo::ptr tmp = new eq_repo();
            push_back(tmp);
        }

        gvector::  gvector() : gvector_(8,as_capacity) { grow(); }

        gvector:: ~gvector() throw() {}


        eq_repo & gvector:: degree(const size_t n) {
            while( size() < n )
                grow();
            return *( (*this)[n] );
        }

    }

}
