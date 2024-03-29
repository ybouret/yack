#include "yack/FLTK/Curve.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace FLTK
    {
        
        Curve:: ~Curve() noexcept {}
        
        Curve:: Curve( const string &id ) :
        name(id),
        color(FL_WHITE)
        {
        }
        
        Curve:: Curve( const char *id ) :
        name(id),
        color(FL_WHITE)
        {
        }
        
        const string & Curve:: key() const noexcept { return name; }
        
        
        Curves:: Curves() noexcept : Curve::DB()
        {}
        
        
        Curves:: ~Curves() noexcept
        {
        }
    
        Curve & Curves:: operator[]( const string &id ) 
        {
            const Curve::Ptr *ppC = search(id);
            if( ppC )
            {
                return coerce(**ppC);
            }
            else 
            {
                Curve::Ptr p( new Curve(id) );
                if( ! insert(p) )
                    throw exception("Curves.insert('%s') unexpected failure!!!", id());
                return *p;
            }
        }

        Curve & Curves:: operator[]( const char *name )
        {
            const string id = name;
            return (*this)[id];
        }
        
        const Curve & Curves:: operator[]( const string &id ) const
        {
            const Curve::Ptr *ppC = search(id);
            if( !ppC )
            {
                throw exception("no Curves['%s']", id() );
            }
            return **ppC;
        }
        
        const Curve & Curves:: operator[]( const char *name ) const
        {
            const string id = name;
            return (*this)[id];
        }
        
        
    }
}

