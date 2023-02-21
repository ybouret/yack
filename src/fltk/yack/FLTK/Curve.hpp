#ifndef YACK_FLTK_CURVE_INCLUDED
#define YACK_FLTK_CURVE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/type/v2d.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/counted.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/associative/hash/set.hpp"

#include <FL/fl_draw.H>

namespace yack
{
    namespace FLTK 
    {
        
        typedef v2d<double>       Point;
        typedef vector<Point>     Points;
        
        class Curve : public Points, public counted
        {
        public:
            explicit Curve(const string &id);
            explicit Curve(const char   *id);
            virtual ~Curve() noexcept;
            
            const string name;
            Fl_Color     color;
            
            const string &key() const noexcept;
            typedef ark_ptr<string,Curve>      Ptr;
            typedef hash_set<string,Curve::Ptr> DB;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Curve);
        };
        
        
        class Curves : public Curve::DB
        {
        public:
            explicit Curves() noexcept;
            virtual ~Curves() noexcept;
            
            Curve & operator[]( const string &id ); //!< existing or create
            const Curve & operator[]( const string &id ) const; //! must exist
            
            Curve & operator[]( const char * );
            const Curve & operator[]( const char *) const;
            
            
        private:
            YACK_DISABLE_COPY(Curves);
        };
        
    }
}

#endif
