
//! \file

#ifndef YACK_AQUEOUS_COLLECTOR_INCLUDED
#define YACK_AQUEOUS_COLLECTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace aqueous
    {

        class survey
        {
        public:
            double   extent;      //!< limiting extent
            sp_repo  vanish;      //!< making these species vanish
            double   request;     //!< minimal request
            sp_repo  nullify;     //!< to nullify those species

            survey(const sp_proxy &proxy) noexcept ;
            ~survey() noexcept;

            void initialize() noexcept;
            void operator()(const actors           &A,
                            const readable<double> &C);
           
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(survey);
            void on_positive(const double xi, const species &s, bool &first);
        };

        class surveys
        {
        public:
            surveys(const sp_proxy &proxy) noexcept :
            reac(proxy),
            prod(proxy)
            {
            }

            ~surveys() noexcept
            {
            }

            survey reac;
            survey prod;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(surveys);
        };


        class collector : public sp_proxy
        {
        public:

            explicit collector(const size_t n) :
            sp_proxy( new sp_zpool() ),
            surv(n,static_cast<const sp_proxy&>(*this))
            {
            }

            virtual ~collector() noexcept {}

            cxx_array<surveys>  surv;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
        };

    }

}

#endif

