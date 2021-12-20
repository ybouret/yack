
//! \file

#ifndef YACK_IOS_FMT_ALIGN_INCLUDED
#define YACK_IOS_FMT_ALIGN_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! alignement of string/text
        //
        //______________________________________________________________________
        class align
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! justification
            enum justify
            {
                left,     //!< left, default
                center,   //!< centered
                right     //!< right
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            align(const string &, const size_t w, const justify how=align::left) throw(); //!< setup with string
            align(const char   *, const size_t w, const justify how=align::left) throw(); //!< setup with text
            align(const align &) throw();                                                 //!< copy
            ~align() throw();                                                             //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &os, const align &a); //!< display


        private:
            YACK_DISABLE_ASSIGN(align);
            const char   * const entry;
            const size_t         count;
            const size_t         width;
            const justify        jflag;
            void        out(std::ostream &) const;
            static void spc(std::ostream &,size_t);
            std::ostream & display(std::ostream &) const; 

        };

    }

}

#endif
