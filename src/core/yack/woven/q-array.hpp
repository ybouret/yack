
//! \file

#ifndef YACK_WOVEN_Q_ARRAY_INCLUDED
#define YACK_WOVEN_Q_ARRAY_INCLUDED 1

#include "yack/woven/metrics.hpp"
#include "yack/apex.hpp"

namespace yack
{
    namespace woven
    {
        class qvector;

        class q_array : public metrics, public readable<apq>
        {
        public:
            virtual ~q_array() noexcept;

            virtual size_t      size()             const noexcept;
            virtual const apq & operator[](size_t) const noexcept;

            static bool check_not_null(const readable<apq> &) noexcept;

            qvector *next;
            qvector *prev;

        protected:
            explicit q_array(const size_t dims);
            void      finalize();

            apq   *coord;
            apz   *znrm2;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(q_array);
            void init();
            void quit(size_t) noexcept;

            void  *entry;
            size_t bytes;
        };

       
    }

}

#endif

