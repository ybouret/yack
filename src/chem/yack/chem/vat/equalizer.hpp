//! \file

#ifndef YACK_CHEM_VAT_EQUALIZER_INCLUDED
#define YACK_CHEM_VAT_EQUALIZER_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    namespace Chemical
    {


        class Cursor : public Species::CoopRepo
        {
        public:
            typedef coop_list<Cursor> List;
            typedef List::node_type   Node;
            typedef List::zpool_type  Bank;
            typedef List::proxy_type  Fund;

            //! initialize with first species, c>=0
            explicit Cursor(const double         c,
                            const Actor         &a,
                            const Species::Fund &f);
            
            virtual ~Cursor() noexcept;

            const Extended::Real xi;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cursor);
        };

        class Cursors : public Cursor::List
        {
        public:
            explicit Cursors(const Cursor::Fund  &,
                             const Species::Fund &) noexcept;
            virtual ~Cursors()                     noexcept;

            void operator()(const double c, const Actor &a);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cursors);
            Species::Fund speciesFund;
        };


        //______________________________________________________________________
        //
        //
        //! Concentrations equalizer
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equalizer();
            virtual ~Equalizer() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void run(const xmlog      &xml,
                     writable<double> &C0,
                     const Cluster    &cl);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Species::Fund   speciesFund;
            Extended::Adder xadd;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };

    }

}

#endif
