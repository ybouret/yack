#include "yack/chem/equilibrium.hpp"
#include "yack/exception.hpp"
#include "yack/type/utils.hpp"
#include "yack/arith/gcd.h"

namespace yack
{
    //using namespace math;

    namespace chemical
    {
        namespace
        {
            class xpair
            {
            public:
                inline xpair(const int a_, const int b_) throw() :
                a(a_), b(b_)
                {

                }

                inline ~xpair() throw()
                {
                }

                inline xpair(const xpair &xp) throw() : a(xp.a), b(xp.b) {}


                static xpair from(const int acof, const int bcof)  throw()
                {
                    assert(acof!=0);
                    assert(bcof!=0);
                    uint64_t ca = absolute(bcof);
                    uint64_t cb = absolute(acof);
                    yack_simplify64(&ca,&cb);
                    return xpair(int(ca),int(cb));
                }

                const int a;
                const int b;

                friend inline std::ostream & operator<<(std::ostream &os, const xpair &xp)
                {
                    os << '(' << xp.a << ',' << xp.b << ')';
                    return os;
                }

            private:
                YACK_DISABLE_ASSIGN(xpair);
            };
        }


        void equilibrium:: inter(const readable<int> &A, const readable<int> &B)
        {
            assert(A.size()==B.size());
            const size_t M = A.size();

            vector<xpair> sub;
            vector<xpair> add;
            std::cerr << "\tA=" << A << std::endl;
            std::cerr << "\tB=" << B << std::endl;

            for(size_t j=M;j>0;--j)
            {
                const int       aa = A[j];
                const int       bb = B[j];
                const sign_type sa = __sign::of(aa);
                const sign_type sb = __sign::of(bb);
                switch( __sign::pair(sa,sb) )
                {
                    case zp_pair:
                    case zn_pair:
                    case zz_pair:
                    case pz_pair:
                    case nz_pair:
                        break;

                    case nn_pair:
                    case pp_pair: {
                        //std::cerr << "\tcan sub with " << aa << "/" << bb << std::endl;
                        const xpair xp = xpair::from(aa,bb);
                        sub << xp;
                    } break;

                    case np_pair:
                    case pn_pair: {
                        //std::cerr << "\tcan add with " << aa << "/" << bb << std::endl;
                        const xpair xp = xpair::from(aa,bb);
                        add << xp;
                    } break;
                }
            }

            std::cerr << "\t\tadd=" << add << std::endl;
            std::cerr << "\t\tsub=" << sub << std::endl;



        }
    }

}
