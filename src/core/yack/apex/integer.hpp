
//! \file

#ifndef YACK_APEX_INTEGER_INCLUDED
#define YACK_APEX_INTEGER_INCLUDED 1

#include "yack/apex/natural.hpp"

namespace yack
{
    
    namespace apex
    {
        
        class integer : public number
        {
        public:
            static const char clid[];
            
            const sign_type s;
            const natural   n;
            
            virtual ~integer() throw();
            integer();
            integer(const int_type z);
            integer(const integer &z);
            integer(const sign_type, const natural &);
            
            integer & operator=(const integer &);
            integer & operator=(const int_type z);
            
            void xch(integer &) throw();
            
            virtual const char * class_uid()       const throw();
            virtual size_t       serialize(ios::ostream &) const;
            
            friend std::ostream &operator<<(std::ostream &, const integer &);
            
            integer operator+() const;
            integer operator-() const;
            
            friend integer operator+(const integer &lhs, const integer &rhs);
            
            
        private:
            
        };
    }
    
    typedef apex::integer apz;
    
}

#endif
        
