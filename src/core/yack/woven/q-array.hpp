
//! \file

#ifndef YACK_WOVEN_Q_ARRAY_INCLUDED
#define YACK_WOVEN_Q_ARRAY_INCLUDED 1

#include "yack/woven/metrics.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/apex.hpp"

namespace yack
{
    namespace woven
    {
        class qvector;

        //______________________________________________________________________
        //
        //
        //! base class for qvectors (handle memory)
        //
        //______________________________________________________________________
        class q_array : public metrics, public readable<apq>, public dynamic
        {
        public:
            //__________________________________________________________________
            //
            //  methods
            //__________________________________________________________________
            virtual            ~q_array()                noexcept;              //!< cleanup
            virtual size_t      size()             const noexcept;              //!< dimensions
            virtual size_t      granted()          const noexcept;              //!< granted memory
            virtual const apq & operator[](size_t) const noexcept;              //!< coordinate
            static  bool        check_not_null(const readable<apq> &) noexcept; //!< helper

            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            qvector *next; //!< for list
            qvector *prev; //!< for list

        protected:
            explicit q_array(const size_t dims); //!< setup
            void     finalize();                 //!< make univocal and compute |coord|^2

            apq   *coord; //!< memory for coodinates [1..dimensions]
            apz   *znrm2; //!< memory for |coord|^2
            
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

