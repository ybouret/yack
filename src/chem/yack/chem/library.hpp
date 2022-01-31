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
            // definitions
            //__________________________________________________________________
            static const char separator = ':'; //!< parsing separator

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

            //! check/insert
            const species & check(const species::pointer &sp);

            //! on the fly species with name+charge
            template <typename NAME>
            const species & operator()(const NAME &name, const unit_t z)
            {
                const species::pointer ptr = new species(name,z);
                return check(ptr);
            }


            const species & operator()(const string &expr); //!< parse a species
            const species & operator()(const char   *expr); //!< parse a species
            void            load(const string &splist);     //!< parse list of species
            void            load(const char   *splist);     //!< parse list of species

            virtual size_t size() const throw(); //!< number of registered species
            const snode   *head() const throw(); //!< fist species knot

            const species & operator[](const string &) const; //!< access
            const species & operator[](const char   *) const; //!< access


            //! fill a trial set of concentrations
            void fill(writable<double> &C,
                      const double      zabove,
                      randomized::bits &ran) throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const library &);

            //! display named values
            template <typename T> inline
            void operator()(std::ostream &os, const readable<T> &arr) const
            {
                os << '{' << std::endl;
                for(const snode *node=head();node;node=node->next)
                {
                    const species &sp = ***node; assert(sp.indx>=1); assert(sp.indx<=size());
                    os << ' '; display(os,sp);
                    os << " = " << arr[sp.indx] << std::endl;
                }
                os << '}' << std::endl;
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species::set db;
            void display(std::ostream &) const;
            void display(std::ostream &, const species &) const;

        public:
            const size_t width; //!< species name max size

        };
    }

}

#endif

