//! \file

#ifndef YACK_CHEMICAL_ENTITY_INCLUDED
#define YACK_CHEMICAL_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/counted.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! named entity with index
        //
        //______________________________________________________________________
        class entity : public large_object, public counted
        {
        public:
            static bool verbose;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~entity() throw(); //!< cleanup
            entity(const entity &);    //!< copy

            //! setup with name+indx
            template <typename NAME> inline
            explicit entity(const NAME  &uid,
                            const size_t idx) :
            large_object(),
            counted(),
            name(uid),
            indx(idx)
            {
                assert(indx>0);
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t operator*()   const throw(); //!< index
            const string & key() const throw(); //!< name

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier

        private:
            const size_t indx; //!< index
            YACK_DISABLE_ASSIGN(entity);
        };
    }

}

#endif
