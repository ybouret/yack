
//! \file

#ifndef YACK_APEX_NPD_INCLUDED
#define YACK_APEX_NPD_INCLUDED 1

#include "yack/apex/natural.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace apex
    {
        //______________________________________________________________________
        //
        //
        //! prime node
        //
        //______________________________________________________________________
        class pnode : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit pnode(const natural &, const size_t) noexcept; //!< setup
            virtual ~pnode()     noexcept;                          //!< cleanup
            pnode(const pnode &) noexcept;                          //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const pnode &); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const natural * const p;    //!< pointer to a persistent prime
            const size_t          n;    //!< it's power > 0
            pnode                *next; //!< for list
            pnode                *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(pnode);
        };

        //______________________________________________________________________
        //
        //
        //! natural prime decomposition
        //
        //______________________________________________________________________
        class npd
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            npd(const natural &);              //!< setup from natural
            npd(const uint_type);              //!< setup from unsigned
            npd() noexcept;                     //!< setup empty a.k.a zero
            npd(const npd &);                  //!< copy
            npd & operator=(const npd &other); //!< assign 
            virtual ~npd() noexcept;            //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const list_of<pnode> & operator*()  const noexcept; //!< access
            const list_of<pnode> * operator->() const noexcept; //!< access

            //! display
            friend std::ostream & operator<<(std::ostream &, const npd &);

        private:
            cxx_list_of<pnode> plist;
        };
    }
}


#endif

