
//! \file

#ifndef YACK_CHEMICAL_LIBRARY_INCLUDED
#define YACK_CHEMICAL_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/chem/gathering.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace randomized { class bits; }

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        // types for library
        //
        //______________________________________________________________________
        typedef suffix_set<string,const species::pointer> species_set; //!< alias
        typedef species_set::knot_type                    snode;       //!< alias


        //______________________________________________________________________
        //
        //
        //! library of unique species
        //
        //______________________________________________________________________
        class library : public gathering, public readable<const species>
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[];           //!< chemical::library
            static const int  min_exp10 = -20;  //!< for concentration
            static const int  max_exp10 =   1;  //!< for concentration
            static double    concentration(randomized::bits &) noexcept; //!< random concentration

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit library() noexcept;    //!< setup
            virtual ~library() noexcept;    //!< cleanup
            library(const library &other); //!< hard copy

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t size() const noexcept;                           //!< number of species
            virtual const_type & operator[](const size_t i) const noexcept; //!< slow access

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void cfill(writable<double> &C, randomized::bits &ran) const; //!< fill random values C.size() >= size()
            
            const   snode *head() const noexcept; //!< sequential access

            //! manual creation of a NEW species
            template <typename NAME> inline
            const species & operator()(const NAME &name, const int z)
            {
                return use( new species(name,z,size()+1) );
            }


            const species &operator()(const string &expr);        //!< on the fly parsing of new/existing species
            const species &operator()(const char   *expr);        //!< on the fly parsing of new/existing species
            const species *look_up(const string &) const noexcept; //!< no-throw look up a species by name
            const species & operator[](const string &) const;     //!< access existing species by name
            const species & operator[](const char   *) const;     //!< access existing species by name

            library & operator<<(const string &); //!< colon-separated list of species
            library & operator<<(const char   *); //!< colon-separated list of species
            
            //! display species+info
            friend std::ostream & operator<<(std::ostream &os, const library &lib);

            //! display helper
            template <typename PREFIX, typename ARRAY> inline
            void operator()(std::ostream &os, const PREFIX &prefix, ARRAY &arr) const
            {
                os << '{' << std::endl;
                for(const snode *node=head();node;node=node->next)
                {
                    const species &s = ***node;
                    const size_t   j = *s;
                    pad(os << ' ' << prefix << '[' << s.name << ']',s) << " = " << arr[j] << std::endl;
                }
                os << '}' << std::endl;
            }

            //! access by name
            template <typename ARRAY, typename NAME> inline
            typename ARRAY::mutable_type &operator()(ARRAY &arr, const NAME &name) const
            {
                const library &self = *this;
                const species &whom = self[name];
                return arr[*whom];
            }
            
            //! access by name
            template <typename ARRAY, typename NAME> inline
            typename ARRAY::const_type &operator()(const ARRAY &arr, const NAME &name) const
            {
                const library &self = *this;
                const species &whom = self[name];
                return arr[*whom];
            }

            //! save frame of concentrations
            std::ostream & frame(std::ostream &os, const readable<double> &C) const;

            //! save frame of concentrations
            ios::ostream & frame(ios::ostream &os, const readable<double> &C) const;

        private:
            YACK_DISABLE_ASSIGN(library);
            species_set sdb;
            const species & use(species *s);

        };



    }

}

#endif
