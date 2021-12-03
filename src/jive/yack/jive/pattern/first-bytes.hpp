
//! \file

#ifndef YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED
#define YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"
#include <iosfwd>

namespace yack
{
    namespace jive
    {
        //__________________________________________________________________
        //
        //
        //! domain of bytes
        //
        //__________________________________________________________________
        class domain : public object
        {
        public:
            //______________________________________________________________
            //
            // types and definitions
            //______________________________________________________________

            //! relative position of domains
            enum position
            {
                before,     //!< separeted, before
                overlap,    //!< overlapping
                after       //!< separated, after
            };


            //______________________________________________________________
            //
            // C++
            //______________________________________________________________
            explicit domain(const uint8_t, const uint8_t) throw(); //!< [lower:upper]
            explicit domain(const uint8_t) throw();                //!< [lower:lower]
            virtual ~domain() throw();                             //!< cleanup
            domain(const domain &other) throw();                   //!< copy
            
            //______________________________________________________________
            //
            // methods
            //______________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const domain &);

            //! relative lhs / rhs
            static position compare(const domain *lhs, const domain *rhs) throw();

            //! check ownership of byte
            bool owns(const uint8_t) const throw();

            //______________________________________________________________
            //
            // members
            //______________________________________________________________
            domain       *next;  //!< for list
            domain       *prev;  //!< for list
            const uint8_t lower; //!< lower bound
            const uint8_t upper; //!< upper bound

        private:
            YACK_DISABLE_ASSIGN(domain);
        };

        //__________________________________________________________________
        //
        //
        //! alias for list of domains
        //
        //__________________________________________________________________
        typedef cxx_list_of<domain> domains;


        //__________________________________________________________________
        //
        //
        //! list of separated, ordered domains
        //
        //__________________________________________________________________
        class first_bytes
        {
        public:
            //______________________________________________________________
            //
            // C++
            //______________________________________________________________
            first_bytes()           throw(); //!< setup
            ~first_bytes()          throw(); //!< cleanup
            first_bytes(const first_bytes&); //!< hard copy

            //______________________________________________________________
            //
            // methods
            //______________________________________________________________
            void grow(domain *dom)           throw();          //!< grow with a single domain
            void grow(list_of<domain> &doms) throw();          //!< grow with other domains
            void full();                                       //!< set to 1 domain 0x00->0xff
            void kill() throw();                               //!< release content
            void add(const uint8_t);                           //!< grow( new domain(a)   )
            void add(const uint8_t, const uint8_t);            //!< grow( new domain(a,b) )
            void sub(const uint8_t);                           //!< remove a byte
            const list_of<domain> & operator*() const throw(); //!< access

            void merge(first_bytes &other) throw();
            void exclude(first_bytes &other);

            //! display
            friend std::ostream & operator<<(std::ostream &, const first_bytes &);


        private:
            YACK_DISABLE_ASSIGN(first_bytes);
            domains self;
            bool is_valid() const throw();
        };



    }
}

#endif

