//! \file

#ifndef YACK_CHEM_BOUNDARY_INCUDED
#define YACK_CHEM_BOUNDARY_INCUDED 1

#include "yack/chem/frontier.hpp"
#include "yack/sequence/cxx-series.hpp"
namespace yack
{
    namespace chemical
    {

        class boundary : public frontier
        {
        public:
            explicit boundary(const sp_fund &) throw();
            virtual ~boundary() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundary);
        };

        class boundaries : public readable<boundary>
        {
        public:
            explicit boundaries(const size_t m, const sp_fund &);
            virtual ~boundaries() throw();

            virtual size_t           size()                   const throw();
            virtual const boundary & operator[](const size_t) const throw();
            void                     destroy() throw();
            void                     upgrade(const double x, const species &s);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
            cxx_series<boundary> bnd;
            sp_fund              bio;
        };


    }
    
}


#endif


