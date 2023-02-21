#include "yack/chem/reactor/balance/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack {

    using namespace math;

    namespace chemical
    {

        sq_repo::  sq_repo(const size_t n) : sq_repo_(n) {}
        sq_repo:: ~sq_repo() noexcept {}


        const char * equalizer:: status_text(const status s) noexcept
        {
            switch(s)
            {
                case balanced: return "balanced";
                case bad_reac: return "bad_reac";
                case bad_prod: return "bad_prod";
                case bad_both: return "bad_both";
            }
            return yack_unknown;
        }

        equalizer:: ~equalizer() noexcept {}


        equalizer:: equalizer(const reactor &_) :
        sp_fund( new sp_bank() ),
        cs(_),
        mx(cs.max_actors()),
        wall( *this ),
        reac(mx,*this),
        prod(mx,*this),
        eqdb(),
        pick(cs.N),
        Ceqz(cs.L,cs.L?cs.M:0),
        gain(cs.L,0.0),
        xadd()
        {
        }

      
        
       

       


    }

}
