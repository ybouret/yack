
#include "yack/chem/xlimits.hpp"
#include "yack/chem/actor.hpp"

namespace yack
{
    namespace chemical
    {
        static inline
        limitation deduce_limitation(const xlimit *reac, const xlimit *prod) throw()
        {
            if(reac)
            {
                if(prod)
                {
                    return limited_by_both;
                }
                else
                {
                    assert(NULL==prod);
                    return limited_by_reac;
                }
            }
            else
            {
                assert(NULL==reac);
                if(prod)
                {
                    return limited_by_prod;
                }
                else
                {
                    assert(NULL==prod);
                    return limited_by_none;
                }
            }
        }
        
        xlimits:: xlimits(const xlimit *rx,
                          const xlimit *px,
                          const size_t  wm) throw() :
        reac(rx),
        prod(px),
        type( deduce_limitation(reac,prod) ),
        wmax(wm)
        {
        }
        
        static inline void  show_id(std::ostream &os,
                                    const actor  &a,
                                    const size_t  w)
        {
            const string &name = (*a).name;
            os << '[' << name << ']';
            for(size_t i=name.size();i<w;++i) os << ' ';
        }
        
        static inline void show_sp(std::ostream &os, size_t n)
        {
            while(n-- > 0) os << ' ';
        }
        
        std::ostream & operator<<(std::ostream &os, const xlimits &lm)
        {
            static const char   pfx[] = "limited by ";
            static const char   sep[] = " : ";
            static const size_t nsp   = 12;
            
            switch(lm.type)
            {
                case limited_by_none:
                    os << pfx << "none";
                    os << ".";
                    break;
                    
                case limited_by_reac:
                    os << pfx << "reac ";
                    show_id(os,lm.reac->id,lm.wmax);
                    show_sp(os,nsp+lm.wmax);
                    os << sep;
                    os << " <=  " << (lm.reac->xi);
                    break;
                    
                case limited_by_prod:
                    os << pfx << "prod ";
                    show_id(os,lm.prod->id,lm.wmax);
                    show_sp(os,nsp+lm.wmax);
                    os << sep;
                    os << " >=  " << -(lm.prod->xi);
                    break;
                    
                case limited_by_both:
                    os << pfx << "reac ";
                    show_id(os,lm.reac->id,lm.wmax);

                    os << " and prod ";
                    show_id(os,lm.prod->id,lm.wmax);
                    os << sep;
                    os << " in " << '[' <<-(lm.prod->xi) << ":" << lm.reac->xi << ']';
                    break;

            }
            
            return os;
        }

        bool xlimits:: acceptable(const double xx) const throw()
        {
            switch(type)
            {
                case limited_by_none:
                    break;

                case limited_by_reac:
                    return xx < reac->xi;

                case limited_by_prod:
                    return xx > -(prod->xi);

                case limited_by_both:
                    return (xx < reac->xi) && (xx > -(prod->xi) );
            }
            return true;
        }

    }
    
}
