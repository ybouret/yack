//! \file

#ifndef YACK_IOS_ENTITY_INCLUDED
#define YACK_IOS_ENTITY_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! entity has a name
        //
        //______________________________________________________________________
        class entity
        {
        public:
            //__________________________________________________________________
            //
            //C++
            //__________________________________________________________________
            template <typename UUID> inline
            explicit entity(const UUID &uuid) : name(uuid) {} //!< setup name
            virtual ~entity() noexcept;                       //!< cleanup
            entity(const entity &);                           //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string &key() const noexcept; //!< name

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< fixed name

        private:
            YACK_DISABLE_ASSIGN(entity);

        public:
            //__________________________________________________________________
            //
            //
            //! using a group of entities
            //
            //__________________________________________________________________
            class group
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit group()    noexcept; //!< setup
                virtual ~group()    noexcept; //!< cleanup
                group(const group&) noexcept; //!< copy

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void update(const entity &) noexcept; //!< grow maxlen

                //! padding output
                template <typename OSTREAM> inline
                OSTREAM & pad(OSTREAM &os, const entity &en) const {
                    for(size_t i=en.name.size();i<maxlen;++i) os << ' ';
                    return os;
                }

                const size_t maxlen; //!< max length of entities name

            private:
                YACK_DISABLE_ASSIGN(group);
            };

        };

    }
}

#endif


