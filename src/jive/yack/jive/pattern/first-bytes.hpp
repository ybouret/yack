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
        class pattern;

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
            domain(const domain &other) throw();                   //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const domain &);

            //! relative lhs / rhs
            static position compare(const domain *lhs, const domain *rhs) throw();

            //! check ownership of byte
            bool owns(const uint8_t) const throw();
            

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            domain       *next;  //!< for list
            domain       *prev;  //!< for list
            const uint8_t lower; //!< lower bound
            const uint8_t upper; //!< upper bound

        private:
            YACK_DISABLE_ASSIGN(domain);
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
        class first_bytes : public domains
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            first_bytes()           throw(); //!< setup
            ~first_bytes()          throw(); //!< cleanup
            first_bytes(const first_bytes&); //!< hard copy


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void          all();                                     //!< set to 1 domain 0x00->0xff
            first_bytes & operator<<( domain *dom ) throw();         //!< include a new domain
            first_bytes & operator<<(list_of<domain> &doms) throw(); //!< include by stealing domains
            first_bytes & operator-=(const uint8_t);                 //!< exclude single byte
            first_bytes & operator-=(list_of<domain> &doms);         //!< exclude all (stolen) domains
            first_bytes & inverse();                                 //!< all - this
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const first_bytes &);

            //! append singulets/intervals to list
            void build(list_of<pattern> &) const;

        private:
            YACK_DISABLE_ASSIGN(first_bytes);
            void add(domain *dom) throw(); //!< grow with a single domain
            void sub(const uint8_t);        //!< remove a byte
            bool is_valid() const throw();
        };



    }
}

#endif

