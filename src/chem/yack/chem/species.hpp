//! \file

#ifndef YACK_CHEMICAL_SPECIES_INCLUDED
#define YACK_CHEMICAL_SPECIES_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/proto-ostream.hpp"

namespace yack
{
    namespace chemical
    {


        //______________________________________________________________________
        //
        //
        //! species=name+algebraic charge
        //
        //______________________________________________________________________
        class species : public entity
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            typedef ark_ptr<string,const species> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() throw(); //!< cleanup
            species(const species &);   //!< copy

            //! initialize with valid index
            template <typename NAME> inline
            explicit species(const NAME   &uid,
                             const int     zzz,
                             const size_t  idx) :
            entity(uid,idx), z(zzz), rank(0)
            {
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            YACK_PROTO_OSTREAM(species); //!< display as name
            string to_string() const;    //!< '[name]'

            //! write formated concentration
            std::ostream &  frame(std::ostream &os, const readable<double> &C) const;

            //! write formated concentration
            ios::ostream &  frame(ios::ostream &os, const readable<double> &C) const;

            //! sort by increasing index
            static inline int compare(const species &lhs, const species &rhs) throw()
            {
                return comparison::increasing(*lhs,*rhs);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int    z;    //!< algebraic charge
            const size_t rank; //!< number of use
            
        private:
            YACK_DISABLE_ASSIGN(species);
        };

        typedef core_repo<const species> sp_repo_;     //!< base class for repository
        typedef sp_repo_::node_type      sp_node;      //!< alias


        //______________________________________________________________________
        //
        //
        //! repository of species
        //
        //______________________________________________________________________
        class sp_repo : public object, public sp_repo_
        {
        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            typedef auto_ptr<const sp_repo> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit sp_repo() throw() : object(), sp_repo_() {} //!< setup
            virtual ~sp_repo() throw() {}                        //!< cleanup

            //! sort contained nodes by increasing species index
            void sort() throw()
            {
                sort_with( species::compare );
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_repo);
        };


    }
}

#endif

