//! \file

#ifndef YACK_CHEM_BOUNDARY_INCUDED
#define YACK_CHEM_BOUNDARY_INCUDED 1

#include "yack/chem/balancing/frontier.hpp"
#include "yack/sequence/cxx-series.hpp"
namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! boundary species with their common extent
        //
        //______________________________________________________________________
        class boundary : public frontier
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit boundary(const sp_fund &) throw(); //!< setup
            virtual ~boundary() throw();                //!< cleanup


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundary);
        };


        //______________________________________________________________________
        //
        //
        //! ordered boundaries
        //
        //______________________________________________________________________
        class boundaries : public readable<boundary>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit boundaries(const size_t m, const sp_fund &); //!< setup
            virtual ~boundaries() throw();                        //!< cleanup

            //__________________________________________________________________
            //
            // interface: readable
            //__________________________________________________________________
            virtual size_t           size()                   const throw(); //!< active boundaries
            virtual const boundary & operator[](const size_t) const throw(); //!< access in increasing ordere

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                     destroy() throw(); //!< destroy boundaries

            //! upgrade boundaries with a new extent/species
            void                     upgrade(const double x, const species &s);

            //! check consistency
            bool is_consistent() const throw();

            //! analyze how much extent is possible
            /**
             - if mark is too small, it's mark
             - otherwise, it the smallest boudary below mark
             - equalities are taken care of
             */
            void analyze(frontier       &zero,
                         const frontier &mark) const;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
            cxx_series<boundary> bnd;
            sp_fund              bio;

            void ins0(const double x, const species &s);
            void ins1(const double x, const species &s);
            void insN(const double x, const species &s);
            

        };


    }
    
}


#endif


