//! \file

#ifndef YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED
#define YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
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
            explicit domain(const uint8_t, const uint8_t) noexcept; //!< [lower:upper]
            explicit domain(const uint8_t) noexcept;                //!< [lower:lower]
            virtual ~domain() noexcept;                             //!< cleanup
            domain(const domain &other) noexcept;                   //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const domain &);

            //! relative lhs / rhs
            static position compare(const domain *lhs, const domain *rhs) noexcept;

            //! check ownership of byte
            bool owns(const uint8_t) const noexcept;
            

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
        class first_bytes : public object, public counted, public domains
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            first_bytes()           noexcept; //!< setup
            virtual ~first_bytes()  noexcept; //!< cleanup
            first_bytes(const first_bytes&); //!< hard copy


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void          all();                                     //!< set to 1 domain 0x00->0xff
            first_bytes & operator<<( domain *dom ) noexcept;         //!< include a new domain
            first_bytes & operator<<(list_of<domain> &doms) noexcept; //!< include by stealing domains
            first_bytes & operator-=(const uint8_t);                 //!< exclude single byte
            first_bytes & operator-=(list_of<domain> &doms);         //!< exclude all (stolen) domains
            first_bytes & inverse();                                 //!< all - this
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const first_bytes &);

            //! append singulets/intervals to list
            void build(list_of<pattern> &) const;

            //! check if code is within domains
            bool has(const uint8_t code) const noexcept;

        private:
            YACK_DISABLE_ASSIGN(first_bytes);
            void add(domain *dom) noexcept; //!< grow with a single domain
            void sub(const uint8_t);        //!< remove a byte
            bool is_valid() const noexcept;
        };



    }
}

#endif

