
//! \file

#ifndef YACK_JIVE_PATTERN_COUNTING_AT_LEAST_INCLUDED
#define YACK_JIVE_PATTERN_COUNTING_AT_LEAST_INCLUDED 1

#include "yack/jive/pattern/joker/joker.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! counting pattern
        //
        //______________________________________________________________________
        class counting : public joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     clid[];                              //!< "counting"
            static const uint32_t mark = YACK_FOURCC('[','<','>',']'); //!< mark

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            //! always accepted
            virtual bool accept(YACK_JIVE_PATTERN_ARGS) const;

            virtual bool strong() const;               //!< nmin>0 and joker is strong
            virtual void encode(ios::ostream &) const; //!< graphviz


            virtual const char *class_uid() const throw();          //!< clid
            virtual size_t      serialize(ios::ostream &fp) const;  //!< uuid+nmin+nmax+jk

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static pattern *create(const size_t,const size_t,pattern *); //!< create

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t nmin; //!< minimal count
            const size_t nmax; //!< maximal count


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~counting() throw();               //!< cleanup

        protected:
            explicit counting(const size_t, const size_t, pattern*) throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(counting);

        };

    }
}

#endif

