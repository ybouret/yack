
//! \file

#ifndef YACK_CHEM_LIMITING_INCLUDED
#define YACK_CHEM_LIMITING_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //! extent limitation status
        //______________________________________________________________________
        enum limitation
        {
            limited_by_none, //!< no limit
            limited_by_reac, //!< limited by a positive extent
            limited_by_prod, //!< limited by a negative extent
            limited_by_both  //!< limited by a range of extent
        };
      
        //______________________________________________________________________
        //
        //
        //! limiting actor an its extent
        //
        //______________________________________________________________________
        class limiting
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            limiting(const actor *pa_, const double xi_) throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actor   * const pa; //!< actor address
            const double          xi; //!< extent

            friend std::ostream & operator<<(std::ostream &, const limiting &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limiting);
            ~limiting() throw();
        };

    }

}

#endif

