//! \file

#ifndef YACK_CHEMICAL_SPECIES_INCLUDED
#define YACK_CHEMICAL_SPECIES_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/data/list/meta-repo.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! species criterion within the reactor
        //
        //______________________________________________________________________
        enum criterion {
            conserved, //!< in regular equilibri(um|a) only
            unbounded, //!< involved in at least a degenerate equilibrium
            spectator  //!< species will null rank
        };


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
            
            //! write formated concentration
            std::ostream &  frame(std::ostream &os, const readable<double> &C) const;

            //! write formated concentration
            ios::ostream &  frame(ios::ostream &os, const readable<double> &C) const;

            //! display as name
            friend std::ostream & operator<<(std::ostream &os, const species &self)
            {
                os << self.name;
                return os;
            }



            //__________________________________________________________________
            //
            // memners
            //__________________________________________________________________
            const int    z;    //!< algebraic charge
            const size_t rank; //!< number of use
            
        private:
            YACK_DISABLE_ASSIGN(species);
            virtual const char *attr() const;
        };


        typedef meta_list<const species> sp_list;     //!< list of species
        typedef meta_repo<const species> sp_repo;     //!< cache of species
        typedef sp_repo::node_type       sp_node;     //!< node within sp_repo
        int sp_node_compare(const sp_node *, const sp_node *) throw(); //!< compare by increasing index
    }
}

#endif

