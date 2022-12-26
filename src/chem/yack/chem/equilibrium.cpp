
#include "yack/chem/equilibrium.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {
        equilibrium:: ~equilibrium() throw()
        {
        }
        
        
        double equilibrium:: K(double t) const
        {
            const double K_ = getK(t);
            if(K_<=0) throw imported::exception( name(), "K(%g)=%g", t,K_);
            return K_;
        }
        
        double equilibrium:: display_time = 0.0;
        size_t equilibrium:: display_size = 12;
        
        std::ostream & operator<<(std::ostream &os, const equilibrium &eq)
        {
            os << '<' << eq.name << '>';
            for(size_t i=eq.name.size();i<eq.display_size;++i)
            {
                os << ' ';
            }
            os << " : (" << std::setw(15) << eq.K(eq.display_time) << ") ";
            os << eq.content();
            return os;
        }

#define COMMON ",style=\"bold,rounded\""
        const char * equilibrium:: attr() const
        {
            if(reac->size)
            {
                if(prod->size)
                {
                    return ",shape=rectangle" COMMON;
                }
                else
                {
                    return ",shape=invtrapezium" COMMON;
                }
            }
            else
            {
                if(prod->size)
                {
                    return ",shape=trapezium" COMMON;
                }
                else
                {
                    return ",shape=rectangle,style=\"dotted,bold\"";
                }
            }

        }


       

        const components & equilibrium:: content() const throw()
        {
            return *this;
        }
    }
    
}

namespace yack
{
    namespace chemical
    {
        eq_team:: ~eq_team() throw()
        {
        }
        
        eq_team:: eq_team() throw() : object(), eq_team_()
        {
        }
        
        

    }
    
}

#include "yack/chem/library.hpp"

namespace yack
{
    namespace chemical
    {

        void   equilibrium:: vizlink(ios::ostream &fp, const library &lib) const
        {
            for(const actor *a=reac->head;a;a=a->next)
            {
                const species &mine = **a;
                const species &s    = lib[mine.name];
                s.link(fp,this);
                if(a->nu>1)
                {
                    const string label = vformat("%u",unsigned(a->nu));
                    s.add_label(fp << '[',label()) << ']';
                }
                end(fp);
            }

            for(const actor *a=prod->head;a;a=a->next)
            {
                const species &mine = **a;
                const species &s    = lib[mine.name];
                this->link(fp, &s);
                if(a->nu>1)
                {
                    const string label = vformat("%u",unsigned(a->nu));
                    s.add_label(fp << '[',label()) << ']';
                }
                end(fp);
            }
        }

    }

}


