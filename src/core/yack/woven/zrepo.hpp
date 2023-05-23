
//! \file

#ifndef YACK_WOVEN_ZREPO_INCLUDED
#define YACK_WOVEN_ZREPO_INCLUDED 1

#include "yack/woven/zvector.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace woven
    {
        typedef cxx_list_of<zvector> zvectors;

        class zrepo : public metrics, public zvectors
        {
        public:
            explicit zrepo(const size_t dims);
            virtual ~zrepo() noexcept;
            zrepo(const zrepo &);

            void ensure(const qvector          &lhs);
            void ensure(const list_of<qvector> &lhs);
            void sort();


            friend std::ostream & operator<<(std::ostream &, const zrepo &);

        private:
            YACK_DISABLE_ASSIGN(zrepo);
        };

    }

}

#endif

