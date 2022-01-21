//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/latch.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class library : public latch, public collection
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit library();           //!< setup
            virtual ~library() throw();   //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! declare a new species with name+charge
            template <typename NAME>
            const species & operator()(const NAME &name, const unit_t z)
            {
                const species::pointer ptr = new species(name,z);
                decl(ptr);
                return *ptr;
            }

            const species & operator()(const string &expr);
            const species & operator()(const char   *expr);

            virtual size_t       size() const throw(); //!< number of registered species
            const species::knot *head() const throw(); //!< fist species knot

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species::set db;
            void decl(const species::pointer &sp);

        };
    }

}

#endif

