
//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/active.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/eqs/groups.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/math/algebra/crout.hpp"

namespace yack
{
    class xmlog;

    namespace chemical
    {

        //! TODO
        class conserve: public object, public counted, public actors
        {
        public:
            explicit conserve() throw() {}
            virtual ~conserve() throw() {}


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conserve);
        };

        typedef arc_ptr<conserve> constraint; //!< alias


        //______________________________________________________________________
        //
        //
        // aliases
        //
        //______________________________________________________________________
        typedef arrays_of<double>    tableaux; //!< alias
        typedef tableaux::array_type tableau;  //!< alias
        typedef matrix<int>          imatrix;  //!< alias
        typedef matrix<double>       rmatrix;  //!< alias
        class outcome;

        //______________________________________________________________________
        //
        //
        // types for list of involved equilibria
        //
        //______________________________________________________________________
        typedef meta_list<const equilibrium>       islot_; //!< alias for list of involved equilibri(um|a)

        //______________________________________________________________________
        //
        //! derived meta list of equilibria
        //______________________________________________________________________
        class islot : public islot_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit islot() throw() : islot_() {}        //!< setup
            virtual ~islot() throw() {}                   //!< cleanup
            islot(const islot &other) : islot_(other) {}  //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display as list
            friend std::ostream & operator<<(std::ostream &os, const islot &self)
            {
                const node_type *node = self.head;
                if(node)
                {
                    os << "{ " << (**node).name;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ", " << (**node).name;
                    }
                    os << " }";
                }
                else
                {
                    os << "{}";
                }
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(islot);
        };

        typedef islot::node_type                   inode; //!< alias for node of involved equilirium
        typedef vector<const islot,memory::dyadic> slots; //!< alias




        //______________________________________________________________________
        //
        //
        //! reactor
        //
        //______________________________________________________________________
        class reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];  //!< "chemical::reactor"
            static bool      &verbose; //!< entity::verbose
            //!
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~reactor() throw(); //!< cleanup

            explicit reactor(const library    &,
                             const equilibria &,
                             const double     t); //!< setup

            //! balance concentrations
            bool balance(writable<double> &C0);

            //! best effort solving
            bool   solve(writable<double> &C0);

            //! wrapper Hamiltonian(Corg+u*(Cend-Corg)) with full clipping
            double operator()(const double u);

            

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            const library    &  corelib;  //!< initial library
            const equilibria &  singles;  //!< initial equilibria
            const active_list   working;  //!< list of active species
            const size_t        M;        //!< corelib.size
            const size_t        N;        //!< singles.size
            const size_t        L;        //!< lattice.size
            const slots         held_by;  //!< for each species
            const equilibria    couples;  //!< all composite
            const equilibria    lattice;  //!< singles|couples
            const groups        related;  //!< related
            const groups        solving;  //!< all solving combinations
            rmulops             xmul;     //!< for mulops
            raddops             xadd;     //!< for addops
            math::crout<double> solv;     //!< for linear system solving
            vector<constraint>  cnsv;     //!< WIP
            
        private:
            const library     worklib; //!< copy of corelib
            tableaux          mtab;    //!< [M]-sized
            tableaux          ntab;    //!< [N]-sized
            tableaux          ltab;    //!< [L]-sized
            
        public:
            const imatrix      Nu;      //!< [NxM] topology
            imatrix            NuA;     //!< [NXM] topology of non-blocked equilibria
            imatrix            Bal;     //!< [MxN] balancing directions
            rmatrix            Psi;     //!< [NxM] jacobian
            rmatrix            Omega;   //!< [NxN] coupling
            rmatrix            iOmeg;   //!< [NxN] to solve

            tableau           &Corg;    //!< [M] starting point
            tableau           &Cend;    //!< [M] final point
            tableau           &Ctry;    //!< [M] working space
            tableau           &dC;      //!< [M] step
            tableau           &Cbal;    //!< [M] for balancing
            thin_array<int>   beta;     //!< [M] for balancing

            tableau           &K;       //!< [N] pre-computed K
            tableau           &Gamma;   //!< [N] current Gamm
            tableau           &xi;      //!< [N] extent of singles
            tableau           &sigma;   //!< [N] slope of singles
            thin_array<bool>   oshot;   //!< [N] overshoot

            const imatrix      NuL;     //!< [LxM] full topology
            tableau           &Kl;      //!< [L]   pre-computed K
            tableau           &Xl;      //!< [L]   solving extents
            thin_array<bool>   blocked; //!< [L]   blocking state
            rmatrix            Ceq;     //!< [LxM] all single solutions

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;
            void                       buildRelated();                           //!< build global partitions
            void                       makeManifold(const xmlog &);              //!< build all solving partitions
            void                       conservation(const xmlog &);
            void                       conservation2(const xmlog &);

            bool                       returnSolved(writable<double> &C0, const xmlog&); //!< upon successful return
            double                     Hamiltonian(const readable<double> &C);           //!< a molar hamiltonian
            double                     Hamiltonian(const group &g);                      //!< build a Ctry from Corg and a group
            bool                       forwardingC(const xmlog &);                       //!< update Cend from extents
            const readable<double> &   Optimized_C(const double H0);                     //!< Ctry between H(Corg) and Cend
            const equilibrium         *topoSingles(size_t &nrun, const xmlog &);         //!< check topology, return most displaced
            void                       topoLattice(const double H0, const xmlog &xml);
            bool                       hasDominant(const double H0, const xmlog &xml);
            void                       gatherOmega();
            void                       deactivated(const size_t ei);

            bool                       primaryBalance(const xmlog &xml);


        };


    }

}

#endif

