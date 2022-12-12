
//! \file

#ifndef YACK_APEX_NPD_INCLUDED
#define YACK_APEX_NPD_INCLUDED 1

#include "yack/apex/natural.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace apex
    {

        class pnode : public object
        {
        public:
            const natural * const p; //!< pointer to a persistent prime
            const size_t          n; //!< it's power > 0

            pnode *next;
            pnode *prev;

            explicit pnode(const natural &p_, const size_t n_) throw();
            pnode(const pnode &) throw();
            virtual ~pnode() throw();

            friend std::ostream & operator<<(std::ostream &, const pnode &);

        private:
            YACK_DISABLE_ASSIGN(pnode);
        };

        //! natural prime decomposition
        class npd
        {
        public:


            npd(const natural &);
            npd(const uint_type);
            npd() throw();
            npd(const npd &);
            npd & operator=(const npd &other);

            virtual ~npd() throw();

            const list_of<pnode> & operator*() const throw();
            const list_of<pnode> * operator->() const throw();
            
            friend std::ostream & operator<<(std::ostream &, const npd &);

        private:
            cxx_list_of<pnode> plist;
        };
    }
}


#endif

