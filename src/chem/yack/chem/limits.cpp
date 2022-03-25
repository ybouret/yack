
#include "yack/chem/limits.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {
        static inline
        limited_type get_limited_type(const limit *r, const limit *p) throw()
        {
            if(r)
            {
                // limited by reac
                if(p)
                {
                    return limited_by_both;
                }
                else
                {
                    return limited_by_reac;
                }
            }
            else
            {
                // not limited by reac
                if(p)
                {
                    return limited_by_prod;
                }
                else
                {
                    return limited_by_none;
                }
            }
        }

        limits:: limits(const limit *r, const limit *p, const size_t w) throw() :
        reac(r),
        prod(p),
        type( get_limited_type(r,p) ),
        wpad(w)
        {

        }


        void limits:: pad(std::ostream &os, const string &name) const
        {
            for(size_t i=name.size();i<wpad;++i) os << ' ';
        }

        void limits:: spc(std::ostream &os, const size_t n) const
        {
            for(size_t i=wpad+n;i>0;--i) os << ' ';
        }

        std::ostream & operator<<(std::ostream &os, const limits &lm)
        {
            switch(lm.type)
            {
                case limited_by_none:
                    os << "unlimited";
                    break;

                case limited_by_reac: {
                    const string &rname = (***(lm.reac)).name;
                    os << "limited by reac [" << rname << "]";
                    lm.pad(os,rname);
                    lm.spc(os,12);
                    os  << " : xi <= " << lm.reac_extent();
                } break;

                case limited_by_prod: {
                    const string &pname = (***(lm.prod)).name;
                    os << "limited by prod [" << pname << "]";
                    lm.pad(os,pname);
                    lm.spc(os,12);
                    os << " : xi >= " << lm.prod_extent();
                } break;

                case limited_by_both: {
                    const string &rname = (***(lm.reac)).name;
                    const string &pname = (***(lm.prod)).name;
                    os << "limited by reac [" << rname << "]";
                    lm.pad(os,rname);
                    os << " and prod [" << pname << "]";
                    lm.pad(os,pname);
                    os << " : xi in [" <<  lm.prod_extent() << ":" << lm.reac_extent() << "]";
                } break;
            }
            return os;
        }


        double limits:: reac_extent() const throw()
        {
            assert(reac);
            assert(reac->xi>=0);
            return reac->xi;
        }

        double limits:: prod_extent() const throw()
        {
            assert(prod);
            assert(prod->xi>=0);
            const double xi = prod->xi;
            return (xi>0?-xi:0);
        }

        void limits:: clamp(double &xi) const throw()
        {
            switch(type)
            {
                case limited_by_none: break;
                case limited_by_reac: xi = yack::min_of(xi,reac->xi); break;
                case limited_by_prod: xi = yack::max_of(-prod->xi,xi); break;
                case limited_by_both: xi = yack::clamp(-prod->xi,xi,reac->xi); break;
            }

        }


        bool limits:: is_acceptable(const double xi) const throw()
        {
            switch(type)
            {
                case limited_by_none: break;
                case limited_by_reac: return (xi <=  reac->xi);
                case limited_by_prod: return (xi >= -prod->xi);
                case limited_by_both: return (xi <= reac->xi) && (xi >= -prod->xi);
            }

            return true;
        }

        bool limits:: should_reduce(const double xi) const throw()
        {
            switch(type)
            {
                case limited_by_none: break;
                case limited_by_reac: return (xi >=  reac->xi);
                case limited_by_prod: return (xi <= -prod->xi);
                case limited_by_both: return (xi >= reac->xi) || (xi <= -prod->xi);
            }

            return false;
        }


    }

}

