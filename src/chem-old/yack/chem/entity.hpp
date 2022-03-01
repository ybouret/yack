//! \file


#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/large-object.hpp"
#include "yack/ios/gv/vizible.hpp"

namespace yack
{
    namespace chemical
    {

        class compendium;

        //______________________________________________________________________
        //
        //
        //! base class for named/indexed class
        //
        //______________________________________________________________________
        class entity : public large_object, public counted, public ios::vizible
        {
        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< name


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual      ~entity()          throw(); //!< cleanup
            const string &key()       const throw(); //!< name
            size_t        operator*() const throw(); //!< indx

            //! access by (check) index
            template <typename T> inline
            const T & operator()(const readable<T> &arr) const throw()
            {
                assert( index_in(arr) );
                return arr[indx];
            }

            //! access by (check) index
            template <typename T> inline
            T & operator()(writable<T> &arr) const throw()
            {
                assert( index_in(arr) );
                return arr[indx];
            }

            
        protected:
            //! setup
            template <typename NAME> inline
            explicit entity(const NAME &id) :
            large_object(),
            counted(),
            ios::vizible(),
            name(id),
            indx(0)
            {
                
            }

            //! check valid index
            bool          index_in(const collection &) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entity);
            const size_t indx; //!< index
            friend class compendium;
        };

    }
}

#endif

