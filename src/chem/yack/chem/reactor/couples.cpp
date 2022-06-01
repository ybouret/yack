
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



        void reactor::build_couples()
        {
            //equilibria       &repo = coerce(couples);
            vector<coeff>     comb(M,as_capacity);
            size_t            ic = 0;

            //------------------------------------------------------------------
            //
            // loop over all equilibria
            //
            //------------------------------------------------------------------
            for(const enode *xnode=eqs.head();xnode;xnode=xnode->next)
            {
                comb.free();
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

                    YACK_CHEM_PRINTLN("  Testing <" << xeq.name << ">|<" << yeq.name << ">");
                    YACK_CHEM_PRINTLN("  |_"<< xnu << " | " << ynu);

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
                        const coeff st_plus(xnu[j],ynu[j]); if(st_plus.x==0||st_plus.y==0) continue;
                        const coeff st_minus = -st_plus;
                        //YACK_CHEM_PRINTLN("   |_found " << st_plus << " (already=" << comb << ")" );

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

                        //YACK_CHEM_PRINTLN("    |_[" << make_name(xeq.name,yeq.name,st_plus)  << "] @" << K_plus);
                        //YACK_CHEM_PRINTLN("    |_[" << make_name(xeq.name,yeq.name,st_minus) << "] @" << K_minus);

                        {
                            const coeff          st = K_plus > K_minus ? st_plus : st_minus;
                            const string         id = make_name(xeq.name,yeq.name,st);
                            equilibrium::pointer eq = new coupled_equilibrium(id,st,Kx,Ky,++ic);
                            

                            for(size_t j=1;j<=M;++j)
                            {
                                const int nu = xnu[j] * st.x + ynu[j] * st.y;
                                if(!nu) continue;
                                const species &sp = sub[j];
                                (*eq)(sp,nu);
                            }
                            YACK_CHEM_PRINTLN("  | (*) " << eq);
                        }
                    }

                }
            }


        }

    }

}

