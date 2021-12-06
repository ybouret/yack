
//! \file

#ifndef YACK_JIVE_PATTERN_JOKER_OPTIONAL_INCLUDED
#define YACK_JIVE_PATTERN_JOKER_OPTIONAL_INCLUDED 1

#include "yack/jive/pattern/joker/joker.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! optional pattern
        //
        //______________________________________________________________________
        class optional : public joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     clid[];                              //!< "optional"
            static const uint32_t mark = YACK_FOURCC('O','P','T','_'); //!< mark

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            //! always accepted
            virtual bool     accept(YACK_JIVE_PATTERN_ARGS) const;
            virtual bool     strong() const;               //!< never strong
            virtual void     encode(ios::ostream &) const; //!< graphviz
            virtual pattern *clone()                const; //!< clone

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *class_uid() const throw();          //!< clid
            virtual size_t      serialize(ios::ostream &fp) const;  //!< uuid+jk

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static pattern *create(pattern *);    //!< create
            

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~optional() throw();         //!< cleanup
            
        protected:
            explicit optional(pattern*) throw(); //!< setup

        private:
            optional(const optional &) throw(); //!< copy
            YACK_DISABLE_ASSIGN(optional);
        };

    }
}

#endif

