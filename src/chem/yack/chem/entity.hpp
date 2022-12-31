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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static bool verbose; //!< global verbosity

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
            size_t         operator*()         const throw(); //!< index
            const string & key()               const throw(); //!< name

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier


        private:
            YACK_DISABLE_ASSIGN(entity);
            const size_t indx;                    //!< index
        };
    }

}



#endif
