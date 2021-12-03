
//! \file

#ifndef YACK_JIVE_PATTERN_PARTITION_INCLUDED
#define YACK_JIVE_PATTERN_PARTITION_INCLUDED 1

#if 0
#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"
#include <iosfwd>

namespace yack
{
    namespace jive
    {
        namespace hood
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
                YACK_DISABLE_COPY_AND_ASSIGN(domain);
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
            class partition
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit partition()        throw(); //!< setup
                virtual ~partition()        throw(); //!< cleanup
                partition(const partition&) throw(); //!< hard copy

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

                void merge(partition &other) throw();
                void exclude(partition &other);

                //! display
                friend std::ostream & operator<<(std::ostream &, const partition &);


            private:
                YACK_DISABLE_ASSIGN(partition);
                domains self;
                bool is_valid() const throw();
            };


        }
    }
}
#endif

#endif

