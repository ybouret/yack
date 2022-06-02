
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/type/v2d.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{
    //using namespace math;

    namespace chemical
    {

        typedef v2d<int>    coeff;

        static inline double zpower(const double K, const int p) throw()
        {
            assert(p!=0);
            return (p>0) ? ipower<double,int>(K,p) : 1.0/ipower<double,int>(K,-p);
        }

        static inline double K_for(const double Kx, const double Ky, const coeff st) throw()
        {
            //std::cerr << Kx << "^(" << st.x << ") * " << Ky << "^(" << st.x << ")" << std::endl;
            return zpower(Kx,st.x) * zpower(Ky,st.y);
        }

        static inline string make_name(const string &xname, const string &yname, const coeff st)
        {
            string ans;
            {
                const int x = st.x;
                switch(x)
                {
                    case  1:             break;
                    case -1: ans += '-'; break;
                    default:
                        ans += vformat("%d*",x);
                        break;
                }
            }
            ans += xname;
            {
                const int y = st.y;
                switch(y)
                {
                    case  1: ans += '+'; break;
                    case -1: ans += '-'; break;
                    default:
                        if(y>0)
                        {
                            ans += vformat("+%d*",y);
                        }
                        else
                        {
                            assert(y<-1);
                            ans += vformat("%d*",y);
                        }
                        break;
                }
            }
            ans += yname;
            return ans;
        }

        class coupled_equilibrium : public equilibrium
        {
        public:
            inline virtual ~coupled_equilibrium() throw() {}

            inline explicit coupled_equilibrium(const string &id,
                                                const coeff   st_,
                                                const double &Kx_,
                                                const double &Ky_,
                                                const size_t  ic) :
            equilibrium(id,ic),
            st(st_),
            Kx(Kx_),
            Ky(Ky_)
            {

            }

            const coeff   st;
            const double &Kx;
            const double &Ky;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(coupled_equilibrium);
            inline virtual double getK(const double) const
            {
                return K_for(Kx, Ky, st);
            }
        };



        size_t reactor::buildCouples()
        {
            YACK_CHEM_PRINTLN("  <BuildCouples>");
            //equilibria       &repo = coerce(couples);
            vector<coeff>     comb(M,as_capacity);
            size_t            ic = N;

            //------------------------------------------------------------------
            //
            // loop over all equilibria
            //
            //------------------------------------------------------------------
            for(const enode *xnode=eqs.head();xnode;xnode=xnode->next)
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

                    YACK_CHEM_PRINTLN("    <" << xeq.name << "|" << yeq.name << ">");
                    //YACK_CHEM_PRINTLN("  |_"<< xnu << " | " << ynu);

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
                        const coeff shared(xnu[j],ynu[j]);
                        if(shared.x==0||shared.y==0)
                        {
                            continue;
                        }
                        apz mp_alpha = -shared.y;
                        apz mp_beta  =  shared.x;
                        apz::simplify(mp_alpha,mp_beta);
                        const coeff st_plus(mp_alpha.cast_to<int>("alpha"), mp_beta.cast_to<int>("beta"));
                        const coeff st_minus = -st_plus;

                        //------------------------------------------------------
                        //
                        // check existing
                        //
                        //------------------------------------------------------
                        {
                            bool already_exists = false;
                            for(size_t k=comb.size();k>0;--k)
                            {
                                const coeff &cf = comb[k];
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
                        const double K_plus  = K_for(Kx, Ky, st_plus);
                        const double K_minus = K_for(Kx, Ky, st_minus);
                        const coeff  st      = K_plus > K_minus ? st_plus : st_minus;
                        const string id      = make_name(xeq.name,yeq.name,st);
                        equilibrium &eq      = coerce(couples).use( new coupled_equilibrium(id,st,Kx,Ky,++ic) );

                        for(size_t j=1;j<=M;++j)
                        {
                            const int nu = xnu[j] * st.x + ynu[j] * st.y;
                            if(!nu) continue;
                            const species &sp = sub[j];
                            eq(sp,nu);
                        }
                        YACK_CHEM_PRINTLN("      (*) " << eq);
                    }
                    YACK_CHEM_PRINTLN("    <" << xeq.name << "|" << yeq.name << "/>");

                }
            }

            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("    #couples = " << couples.size());
            YACK_CHEM_PRINTLN(couples);
            YACK_CHEM_PRINTLN("  <BuildCouples/>");
            return couples.size();
        }

    }

}

