
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
        //______________________________________________________________________
        //
        //
        //! domain of bytes
        //
        //______________________________________________________________________
        class domain : public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! relative position of domains
            enum position
            {
                before,     //!< separeted, before
                overlap,    //!< overlapping
                after       //!< separated, after
            };


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit domain(const uint8_t, const uint8_t) throw(); //!< [lower:upper]
            explicit domain(const uint8_t) throw();                //!< [lower:lower]
            virtual ~domain() throw();                             //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const domain &);

            //! relative lhs / rhs
            static position compare(const domain *lhs, const domain *rhs) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            domain       *next;  //!< for list
            domain       *prev;  //!< for list
            const uint8_t lower; //!< lower bound
            const uint8_t upper; //!< upper bound

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(domain);
        };

        //______________________________________________________________________
        //
        //
        //! alias for list of domains
        //
        //______________________________________________________________________
        typedef cxx_list_of<domain> domains;


        //______________________________________________________________________
        //
        //
        //! list of separated, ordered domains
        //
        //______________________________________________________________________
        
        class partition : public domains
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit partition()        throw(); //!< setup
            virtual ~partition()        throw(); //!< cleanup
            partition(const partition&) throw(); //!< hard copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void grow(domain *dom)           throw(); //!< grow with a single domain
            void grow(list_of<domain> &doms) throw(); //!< grow with other domains

            void add(const uint8_t);                  //!< grow( new domain(a)   )
            void add(const uint8_t, const uint8_t);   //!< grow( new domain(a,b) )

            //! display
            friend std::ostream & operator<<(std::ostream &, const partition &);
            
            
        private:
            YACK_DISABLE_ASSIGN(partition);
            bool is_valid() const throw();
        };


    }
}

#endif

