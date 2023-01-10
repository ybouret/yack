//! \file

#ifndef YACK_CHEMICAL_ENTITY_INCLUDED
#define YACK_CHEMICAL_ENTITY_INCLUDED 1

#include "yack/chem/indexed.hpp"
#include "yack/string.hpp"
#include "yack/counted.hpp"
#include "yack/large-object.hpp"
#include "yack/ios/gv/vizible.hpp"

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
        class entity :
        public large_object,
        public counted,
        public indexed,
        public ios::vizible
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
            indexed(idx),
            name(uid)
            {
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key()               const throw(); //!< name

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier


        private:
            YACK_DISABLE_ASSIGN(entity);
        };
    }

}



#endif
