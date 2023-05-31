//! \file

#ifndef YACK_CHEM_VAT_CLUSTER_INCLUDED
#define YACK_CHEM_VAT_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/vat/act.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"
#include "yack/randomized/bits.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Cluster of interdependent equilibria
        //
        //______________________________________________________________________
        class Cluster : public spot_object, public EqList, public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Cluster"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Cluster(Equilibrium &first); //!< setup with first equilibrium
            virtual ~Cluster() noexcept;          //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool recruits(const Equilibrium &) const noexcept; //!< true if equilibrium is link to one of self
            bool linkedTo(const Cluster     &) const noexcept; //!< true if at least two equilibria area linked

            //! finalize cluster
            /**
             - build and check topology and local indices
             - discoverLaws()
             - makeManifold()
             - findOutRoles()
             */
            void finalize(const xmlog &xml, Equilibria &eqs);

            //! display components
            friend std::ostream & operator<<( std::ostream &, const Cluster &);

            //! target[SubLevel] = source[TopLevel]
            template <typename TARGET, typename SOURCE> inline
            void load(TARGET &target, SOURCE &source) const
            {
                for(const SpNode *sn=lib.head;sn;sn=sn->next)
                {
                    const Species &sp = ***sn;
                    target[ sp.indx[SubLevel] ] = source[ sp.indx[TopLevel] ];
                }
            }


            //! helper to display info
            template <typename ARRAY> inline
            std::ostream & operator()(std::ostream    & os,
                                      const char      * pfx,
                                      ARRAY           & arr,
                                      const char      * sfx,
                                      const IndexLevel  level)
            {
                if(!pfx) pfx="";
                if(!sfx) sfx="";
                os << '{' << std::endl;
                for(const SpNode *node=lib.head;node;node=node->next)
                {
                    const Species &sp = ***node;
                    pad( os << pfx << sp.name << sfx,sp) << " = " << arr[ sp.indx[level] ] << std::endl;
                }
                os << '}';
                return os;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const SpList           lib;     //!< employed species
            const matrix<int>      Nu;      //!< main topology
            const matrix<unsigned> Qmat;    //!< matrix of constraints
            const size_t           Qrnk;    //!< rank(Qm)
            const ConservationLaws laws;    //!< effective laws
            const Acts             acts;    //!< group of dependent laws
            const SpList           conserved;
            const SpList           unbounded;
            const addrbook         conservedDB;
            const addrbook         unboundedDB;

            const EqList           standard; //!< at least a conserved species on each side
            const EqList           prodOnly; //!< product(s) only
            const EqList           reacOnly; //!< reactant(s) onlu
            const EqList           nebulous; //!< based only on unbounded species

            Cluster               *next;    //!< for list
            Cluster               *prev;    //!< for list
            Gathering              reac;    //!< to display reac
            Gathering              prod;    //!< to display prod
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cluster);
            void discoverLaws(const xmlog &);
            void assembleActs(const xmlog &);
            void findOutRoles(const xmlog &);
            void makeManifold(const xmlog &, Equilibria &eqs);
        };

    }


}

#endif

