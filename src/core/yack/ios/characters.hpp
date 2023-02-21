
//! \file

#ifndef YACK_IOS_CHARACTERS_INCLUDED
#define YACK_IOS_CHARACTERS_INCLUDED 1

#include "yack/ios/character.hpp"
#include "yack/string/fwd.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/concrete.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for characters
        //
        //______________________________________________________________________
        typedef klist<uint8_t,character> characters_;

        //______________________________________________________________________
        //
        //
        //! pool for local cache
        //
        //______________________________________________________________________
        typedef kpool<uint8_t,character> repository;

        //______________________________________________________________________
        //
        //
        //! concrete list of characters
        //
        //______________________________________________________________________
        class characters : public characters_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            characters()          noexcept;              //!< setup empty
            virtual ~characters() noexcept;              //!< cleanup
            characters(const characters &);             //!< copy
            characters & operator=(const characters &); //!< assign by copy swap
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         load(const void *, size_t);    //!< add a block
            characters & operator<<(const char   *msg); //!< add a message
            characters & operator<<(const uint8_t chr); //!< add a byte

            //! display as C-style string
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const characters &chars)
            {
                for(const node_type *node = chars.head;node;node=node->next)
                {
                    os << **node;
                }
                return os;
            }

            //! generic trim of bad chars
            template <typename PROC> inline characters & trim_(PROC &is_bad) noexcept
            {
                while(size && is_bad( **tail ) ) characters_::trim();
                return *this;
            }

            //! generic skip of bad chars
            template <typename PROC> inline characters & skip_(PROC &is_bad) noexcept
            {
                while(size && is_bad( **head ) ) characters_::skip();
                return *this;
            }

            //! generic strip of bad chars
            template <typename PROC> inline characters & strip_(PROC &is_bad) noexcept
            {
                return  trim_<PROC>(is_bad).template skip_<PROC>(is_bad);
            }

            characters & trim(const char *bad, const size_t num)  noexcept; //!< trim
            characters & trim(const char *bad)                    noexcept; //!< trim
            characters & skip(const char *bad, const size_t num)  noexcept; //!< skip
            characters & skip(const char *bad)                    noexcept; //!< skip
            characters & strip(const char *bad, const size_t num) noexcept; //!< strip
            characters & strip(const char *bad)                   noexcept; //!< strip




            char        *cstr()          const; //!< filled from legacy::acquire
            static void  free(char *)  noexcept; //!< free   with legacy::release
            string       to_string()     const; //!< directly to string
            string      *to_new_string() const; //!< to string pointer directly
            size_t       fill(char buffer[],const size_t buflen) noexcept; //!< transfer at most buflen first chars

        private:
            struct is_bad_ {
                const char  *bad;
                const size_t num;
                inline bool operator()(const int c) const noexcept
                {
                    for(size_t i=0;i<num;++i) { if(c==bad[i]) return true; }
                    return false;
                }
            };

            template <typename METHOD_POINTER> inline
            characters & apply(const char *bad, const size_t num, METHOD_POINTER method) noexcept
            {
                assert( yack_good(bad,num) );
                is_bad_ is_bad  = { bad, num };
                return ((*this).*method)(is_bad);
            }
        };

    }

}

#endif

