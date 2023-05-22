
//! \file

#ifndef YACK_WOVEN_QBRANCH_INCLUDED
#define YACK_WOVEN_QBRANCH_INCLUDED 1

#include "yack/woven/qfamily.hpp"

namespace yack
{
    namespace woven
    {

        typedef cxx_list_of<qfamily> qfamilies;
        
        class qbranch : public metrics, public qfamilies
        {
        public:
            static const char clid[]; //!< "qbranch"
            explicit qbranch(const size_t dims);
            virtual ~qbranch() noexcept;

            template <typename T> inline
            void initialize(const matrix<T> &M) {
                initialize( check_dims(M) );
                for(qfamily *f=head;f;f=f->next)
                {
                    if(!f->try_grow(M)) raise_null();
                }
            }

            friend std::ostream & operator<<(std::ostream &, const qbranch &);



            indices::fund fund;

        private:
            YACK_DISABLE_ASSIGN(qbranch);
            size_t check_dims(const matrix_metrics &M) const;
            void   initialize(const size_t rmax);
            void   raise_null() const;
        };

    }
}

#endif
