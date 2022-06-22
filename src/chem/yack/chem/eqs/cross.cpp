
#include "yack/chem/eqs/cross.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{
    namespace chemical
    {
        static inline double zpower(const double K, const int p) throw()
        {
            assert(p!=0);
            return (p>0) ? ipower<double,int>(K,p) : 1.0/ipower<double,int>(K,-p);
        }

        double cross_equilibrium:: K_for(const double Kx, const double Ky, const stoichiometry st) throw()
        {
            return zpower(Kx,st.x) * zpower(Ky,st.y);
        }

        cross_equilibrium:: ~cross_equilibrium() throw() {}

        double cross_equilibrium:: getK(const double) const
        {
            return K_for(Kx, Ky, st);
        }


        string cross_equilibrium:: make_name(const string        &xname,
                                             const string        &yname,
                                             const stoichiometry &st)
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
    }

}

