//! \file

#ifndef YACK_CHEMICAL_ENTITY_INCLUDED
#define YACK_CHEMICAL_ENTITY_INCLUDED 1

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
        class entity : public large_object, public counted, public ios::vizible
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
            ios::ostream & viz(ios::ostream &) const;         //!< write to graphViz

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier

        private:
            const size_t indx; //!< index
            YACK_DISABLE_ASSIGN(entity);
            virtual const char *attr() const = 0; //!< for viz
        };
    }

}



#endif
