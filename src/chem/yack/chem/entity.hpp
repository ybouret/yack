//! \file

#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/string.hpp"
#include "yack/large-object.hpp"
#include "yack/ios/markup.hpp"

namespace yack
{
    namespace chemical
    {

        //! helper for verbosity
#define YACK_CHEM_PRINTLN(MSG) do { if(chemical::entity::verbose) { std::cerr << MSG << std::endl; } } while(false)

        //! helper for verbosity
#define YACK_CHEM_PRINT(MSG) do { if(chemical::entity::verbose) { std::cerr << MSG; } } while(false)

        //! helper for verbosity markers
#define YACK_CHEM_MARKUP(PFX,TAG)  YACK_IOS_MARKUP() (chemical::entity::verbose,std::cerr,PFX,TAG)

        //______________________________________________________________________
        //
        //
        //! indexed and name entity
        //
        //______________________________________________________________________
        class entity : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definition
            //__________________________________________________________________
            static bool verbose; //!< global vebosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename ID> inline
            entity(const ID &uuid, const size_t ipos) :
            name(uuid),
            indx(ipos)
            {
                assert(indx>0);
            }

            //! cleanup
            virtual ~entity() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t        operator*() const throw(); //!< access index
            const string &key()       const throw(); //!< access name

            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< uuid

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entity);
            const size_t indx; //!< uuid
        };


    }
}

#endif

