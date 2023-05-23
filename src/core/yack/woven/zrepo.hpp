
//! \file

#ifndef YACK_WOVEN_ZREPO_INCLUDED
#define YACK_WOVEN_ZREPO_INCLUDED 1

#include "yack/woven/zvector.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace woven
    {
        typedef cxx_list_of<zvector> zvectors; //!< alias

        //______________________________________________________________________
        //
        //
        //! management of zvectors
        //
        //______________________________________________________________________
        class zrepo : public metrics, public zvectors
        {
        public:
            //______________________________________________________________________
            //
            // C++
            //______________________________________________________________________
            explicit zrepo(const size_t dims); //!< setup
            virtual ~zrepo() noexcept;         //!< cleanup
            zrepo(const zrepo &);              //!< copy

            //______________________________________________________________________
            //
            // methods
            //______________________________________________________________________
            void ensure(const qvector          &lhs); //!< check/copy/finalize
            void ensure(const list_of<qvector> &lhs); //!< ensure all in lhs
            void sort();                              //!< sort

            //! display
            friend std::ostream & operator<<(std::ostream &, const zrepo &);

        private:
            YACK_DISABLE_ASSIGN(zrepo);
        };

    }

}

#endif

