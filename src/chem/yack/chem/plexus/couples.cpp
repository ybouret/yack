#include "yack/chem/plexus.hpp"
#include "yack/chem/eqs/cross.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex/integer.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {
        size_t plexus:: buildMatchingCouples()
        {
            YACK_CHEM_MARKUP(vpfx, "buildMatchingCouples");
            vector<stoichiometry> comb(M,as_capacity);
            size_t                 ic = N;

            //------------------------------------------------------------------
            //
            // loop over all equilibria
            //
            //------------------------------------------------------------------
            for(const enode *xnode=singles.head();xnode;xnode=xnode->next)
            {
                const equilibrium   &xeq = ***xnode;
                const size_t         xid = *xeq;
                const readable<int> &xnu = Nu[xid];
                const double        &Kx  = K[xid];

                //--------------------------------------------------------------
                //
                // loop over remaining eq
                //
                //--------------------------------------------------------------
                for(const enode *ynode=xnode->next;ynode;ynode=ynode->next)
                {
                    const equilibrium   &yeq = ***ynode;
                    const size_t         yid = *yeq;
                    const readable<int> &ynu = Nu[yid];
                    const double        &Ky  = K[yid];
                    comb.free();

                    YACK_CHEM_PRINTLN(vpfx << "  {" << xeq.name << "|" << yeq.name << "}");

                    //----------------------------------------------------------
                    //
                    // loop over all coeffs
                    //
                    //----------------------------------------------------------
                    for(size_t j=1;j<=M;++j)
                    {
                        //------------------------------------------------------
                        //
                        // create new coeff
                        //
                        //------------------------------------------------------
                        const stoichiometry shared(xnu[j],ynu[j]);
                        if(shared.x==0||shared.y==0)
                        {
                            continue;
                        }
                        apz mp_alpha = -shared.y;
                        apz mp_beta  =  shared.x;
                        apz::simplify(mp_alpha,mp_beta);
                        const stoichiometry st_plus(mp_alpha.cast_to<int>("alpha"), mp_beta.cast_to<int>("beta"));
                        const stoichiometry st_minus = -st_plus;

                        //------------------------------------------------------
                        //
                        // check existing
                        //
                        //------------------------------------------------------
                        {
                            bool already_exists = false;
                            for(size_t k=comb.size();k>0;--k)
                            {
                                const stoichiometry &cf = comb[k];
                                if( (cf == st_plus) || (cf == st_minus) )
                                {
                                    already_exists = true;
                                    break;
                                }
                            }
                            if(already_exists) continue;
                        }

                        //------------------------------------------------------
                        //
                        // record this coeff
                        //
                        //------------------------------------------------------
                        comb << st_plus;

                        //------------------------------------------------------
                        //
                        // compute constants to choose binary
                        //
                        //------------------------------------------------------
                        const double        K_plus  = cross_equilibrium::K_for(Kx, Ky, st_plus);
                        const double        K_minus = cross_equilibrium::K_for(Kx, Ky, st_minus);
                        const stoichiometry st      = K_plus > K_minus ? st_plus : st_minus;
                        const string        id      = cross_equilibrium::make_name(xeq.name,yeq.name,st);
                        equilibrium        &eq      = coerce(couples).use( new cross_equilibrium(id,st,Kx,Ky,++ic) );

                        for(size_t j=1;j<=M;++j)
                        {
                            const int nu = xnu[j] * st.x + ynu[j] * st.y;
                            if(!nu) continue;
                            const species &sp = sub[j];
                            eq(sp,nu);
                        }
                        //YACK_CHEM_PRINTLN(vpfx << "  (*) " << eq);
                    }
                    //YACK_CHEM_PRINTLN(vpfx << "<" << xeq.name << "|" << yeq.name << "/>");

                }
            }

            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "        #couples = " << couples.size());
            YACK_CHEM_PRINTLN(vpfx << "-------- couples --------" << std::endl << couples);
            return couples.size();
        }
    }

}

