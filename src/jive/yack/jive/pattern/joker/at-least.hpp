//! \file

#ifndef YACK_JIVE_PATTERN_JOKER_AT_LEAST_INCLUDED
#define YACK_JIVE_PATTERN_JOKER_AT_LEAST_INCLUDED 1

#include "yack/jive/pattern/joker/joker.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! at_least pattern
        //
        //______________________________________________________________________
        class at_least : public joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     clid[];                              //!< "at_least"
            static const uint32_t mark = YACK_FOURCC('[','>','=',']'); //!< mark
            static const uint32_t zom_ = YACK_FOURCC('Z','O','M',':'); //!< for zero or more
            static const uint32_t oom_ = YACK_FOURCC('O','O','M',':'); //!< for one  or more

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________

            virtual bool     accept(YACK_JIVE_PATTERN_ARGS) const; //!< num>=count
            virtual bool     strong() const;                       //! count>0 and joker is strong
            virtual void     encode(ios::ostream &) const;         //!< graphviz
            virtual pattern *clone()                const;         //!< clone
            virtual bool     is_univocal()  const throw();         //!< false

            virtual const char *class_uid() const throw();          //!< clid
            virtual size_t      serialize(ios::ostream &fp) const;  //!< uuid+count+jk

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static pattern *create(const size_t,pattern *); //!< create
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t count; //!< minimal count


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~at_least() throw();                      //!< cleanup

        protected:
            explicit at_least(const size_t,pattern*) throw(); //!< setup

        private:
            at_least(const at_least &) throw();
            YACK_DISABLE_ASSIGN(at_least);

        };

        pattern * zero_or_more(pattern *); //!< zero or more pattern
        pattern * one_or_more(pattern *);  //!< one or more pattern

    }
}

#endif

