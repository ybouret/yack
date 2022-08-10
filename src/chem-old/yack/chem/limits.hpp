

//! \file

#ifndef YACK_CHEM_LIMITS_INCLUDED
#define YACK_CHEM_LIMITS_INCLUDED 1

#include "yack/chem/limit.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! limited classification
        //
        //______________________________________________________________________
        enum limited_type
        {
            limited_by_none, //!< no limits
            limited_by_reac, //!< limited by reactants
            limited_by_prod, //!< limited by products
            limited_by_both  //!< limited by both
        };

        //______________________________________________________________________
        //
        //
        //! limit from actors
        //
        //______________________________________________________________________
        class limits
        {
        public:
            limits(const limit *r, const limit *p,const size_t w) throw(); //!< setup

            //! display info
            friend std::ostream & operator<<(std::ostream &, const limits &);

            const limit * const reac; //!< limiting reactant
            const limit * const prod; //!< limiting product
            const limited_type  type; //!< classification
            const size_t        wpad; //!< to display
            
            double reac_extent() const throw(); //!< reac->xi
            double prod_extent() const throw(); //!< -prod->xi

            //! clamp according to limits
            void clamp(double &xi) const throw();

            //! check acceptable value
            bool is_acceptable(const double xi) const throw();

            //! if causes a concentration to become <=0
            bool should_reduce(const double xi) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limits);
            ~limits() throw();
            void pad(std::ostream &, const string &) const;
            void spc(std::ostream &, const size_t n) const; //!< n+wpad
        };

    }

}

#endif


