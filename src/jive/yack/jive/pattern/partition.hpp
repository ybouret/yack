
//! \file

#ifndef YACK_JIVE_PATTERN_PARTITION_INCLUDED
#define YACK_JIVE_PATTERN_PARTITION_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"
#include <iosfwd>

namespace yack
{
    namespace jive
    {

        class domain : public object
        {
        public:
            
            domain       *next;
            domain       *prev;
            const uint8_t lower;
            const uint8_t upper;

            virtual ~domain() throw();
            explicit domain(const uint8_t) throw();
            explicit domain(const uint8_t, const uint8_t) throw();

            friend std::ostream & operator<<(std::ostream &, const domain &);

            enum position
            {
                before,
                overlap,
                after
            };

            static position compare(const domain *lhs, const domain *rhs) throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(domain);
        };

        typedef cxx_list_of<domain> domains;


        //! list of separated, ordered domains
        class partition : public domains
        {
        public:
            explicit partition() throw();
            virtual ~partition() throw();

            void grow(domain *dom) throw();
            void grow(list_of<domain> &doms) throw();

            void add(const uint8_t);
            void add(const uint8_t, const uint8_t);

            friend std::ostream & operator<<(std::ostream &, const partition &);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(partition);
            bool is_valid() const throw();
        };


    }
}

#endif

