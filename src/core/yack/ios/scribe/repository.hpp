

//! \file

#ifndef YACK_IOS_SCRIBE_REPOSITORY_INCLUDED
#define YACK_IOS_SCRIBE_REPOSITORY_INCLUDED 1

#include "yack/ios/scribe.hpp"
#include "yack/singleton.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/data/suffix/tree.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {

        //______________________________________________________________________
        //
        //
        //! database of scribed
        //
        //______________________________________________________________________
        class scribes : public singleton<scribes>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char                               call_sign[];   //!< for singleton
            static const at_exit::longevity                 life_time = 7; //!< for singleton
            typedef arc_ptr<scribe>                         scribe_ptr;    //!< alias
            typedef kernel::suffix_tree<scribe_ptr,uint8_t> scribe_set;    //!< alias
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! object to string
            template <typename T>
            string operator()(const T &args) const
            {
                static const scribe &s = get( typeid(T) );
                return s.to_string(&args);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            string real_format; //!< format for reals


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scribes);
            friend class singleton<scribes>;
            explicit scribes();
            virtual ~scribes() throw();
            void          use(scribe *);
            void          chk(scribe *);
            const scribe &get(const std::type_info &tid) const;
            scribe_set db;
        };

    }

}

#endif

