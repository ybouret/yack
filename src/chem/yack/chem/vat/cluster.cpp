#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/flak.hpp"

namespace yack
{
    namespace Chemical
    {

        const char Cluster:: CLID[] = "Chemical::Cluster";

        Cluster:: ~Cluster() noexcept {}

        Cluster:: Cluster(Equilibrium &first) :
        Equilibrium::CoreRepo(),
        rank(0),
        last(NULL),
        lib(),
        Nu(),
        NuT(),
        Nu0(),
        Qmat(),
        Qrnk(0),
        laws(),
        acts(),

        conserved(),
        unbounded(),
        isRegular(),
        
        standard(),
        prodOnly(),
        reacOnly(),
        nebulous(),

        regulating(),
        equalizing(),

        next(0),
        prev(0),
        reac(),
        prod(),
        spec()
        {
            (*this) << first;
        }


        bool Cluster:: recruits(const Equilibrium &other) const noexcept
        {
            for(const Equilibrium::Node *en=head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                if(eq.linkedTo(other)) return true;
            }
            return false;
        }

        bool Cluster:: linkedTo(const Cluster &other) const noexcept
        {
            for(const Equilibrium::Node *en=head;en;en=en->next)
            {
                const Equilibrium &lhs = ***en;
                for(const Equilibrium::Node *on=other.head;on;on=on->next)
                {
                    const Equilibrium &rhs = ***on;
                    assert(&rhs != &lhs);
                    if(lhs.linkedTo(rhs)) return true;
                }
            }
            return false;
        }

        std::ostream & Cluster:: display(std::ostream &os, const Equilibrium &eq) const
        {
            pad(os << " <" << eq.name << ">",eq) << " : ";
            reac.pad(os << eq.reac.name,eq.reac);
            os << " <=> ";
            prod.pad(os << eq.prod.name,eq.prod);
            os << " : ";
            return os;
        }

        std::ostream & operator<<( std::ostream &os, const Cluster &cl)
        {
            os << '{' << std::endl;
            for(const Equilibrium::Node *node=cl.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                cl.display(os,eq) << std::endl;
            }
            os << '}';
            return os;
        }



        void Cluster:: finalize(const xmlog      &xml,
                                Equilibria       &eqs,
                                Extended::Vector &usr)
        {
            YACK_XMLSUB(xml,"Cluster::finalize");
            assert(0==lib.size);
            
            {
                addrbook ab;

                //--------------------------------------------------------------
                //
                // collect species and renumber eqs
                //
                //--------------------------------------------------------------
                {
                    size_t sub = 1;
                    for(const Equilibrium::Node *en=head;en;en=en->next,++sub)
                    {
                        const Equilibrium &eq = ***en;
                        update(eq);
                        if(eq->size<=0)   throw imported::exception(CLID,"empty <%s>",       eq.name());
                        if(!eq.neutral()) throw imported::exception(CLID,"not neutral <%s>", eq.name());
                        if(!eq.minimal()) throw imported::exception(CLID,"not minimal <%s>", eq.name());
                        eq.submitTo(ab);
                        coerce(eq.indx[SubLevel]) = sub;
                        if(xml.verbose) *xml << " (+) " << eq.name << std::endl;
                    }
                }


                //--------------------------------------------------------------
                //
                // collect lib from renumber species
                //
                //--------------------------------------------------------------
                {
                    size_t sub = 1;
                    for(addrbook::const_iterator it=ab.begin();it!=ab.end();++it,++sub)
                    {
                        const Species &sp = *static_cast<const Species *>(*it);
                        coerce(lib) << sp;
                        coerce(sp.indx[SubLevel]) = sub;
                        spec.update(sp);
                    }
                }
                YACK_XMLOG(xml, " (*) " << lib);
            }

            if(size)
            {
                //--------------------------------------------------------------
                //
                // create principal topology
                //
                //--------------------------------------------------------------
                assert(lib.size>0);
                coerce(Nu).make(size,lib.size);
                for(const Equilibrium::Node *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    eq.fill( coerce(Nu)[ eq.indx[SubLevel] ], SubLevel);
                }
                YACK_XMLOG(xml, " Nu = " << Nu);

                coerce(rank) = apex::flak::rank(Nu);
                if(size!=rank)
                    throw imported::exception(CLID,"system rank=%u < size=%u", unsigned(rank), unsigned(size));

                assert(rank==Nu.rows);
                coerce(NuT).make(Nu.cols,Nu.rows);
                coerce(NuT).assign(Nu,transposed);


                //--------------------------------------------------------------
                //
                // finding out conservation laws
                //
                //--------------------------------------------------------------
                discoverLaws(xml);

                //--------------------------------------------------------------
                //
                // build manifold
                //
                //--------------------------------------------------------------
                makeManifold(xml,eqs,usr);

                const Equilibrium::Node *node = head;
                for(size_t i=rank;i>0;--i) node=node->next;
                coerce(last) = node;

                //--------------------------------------------------------------
                //
                // build full topology
                //
                //--------------------------------------------------------------
                coerce(Nu0).make(size,lib.size);
                for(const Equilibrium::Node *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    eq.fill( coerce(Nu0)[ eq.indx[SubLevel] ], SubLevel);
                }
            }

            //------------------------------------------------------------------
            //
            // find out species and equilibria roles
            //
            //------------------------------------------------------------------
            findOutRoles(xml);


            //------------------------------------------------------------------
            //
            // create tribes
            //
            //------------------------------------------------------------------
            gatherTribes(xml);


        }

        bool Cluster:: isConserved(const Species &sp) const noexcept
        {
            return isRegular[ sp.indx[SubLevel] ];
        }


        bool Cluster:: isUnbounded(const Species &sp) const noexcept
        {
            return !isRegular[ sp.indx[SubLevel] ];
        }


        void Cluster:: displayK(std::ostream &os, const Extended::Vector &K) const
        {

            os << '{' << std::endl;
            for(const Equilibrium::Node *en = head; en; en=en->next)
            {
                const Equilibrium &eq = ***en;
                display(os,eq);
                assert(eq.indx[TopLevel]<=K.size());
                const Extended::Real k = K[ eq.indx[TopLevel] ];
                os << "K = 10^(" << k.exp10() << ")" << std::endl;
            }

            os << '}' << std::endl;
        }

        void Cluster:: displayS(std::ostream &os, const Extended::Vector &S) const
        {

            os << '{' << std::endl;
            for(const Equilibrium::Node *en = head; en; en=en->next)
            {
                const Equilibrium &eq = ***en;
                display(os,eq);
                assert(eq.indx[TopLevel]<=S.size());
                const Extended::Real s = S[ eq.indx[TopLevel] ];
                os << "S = 10^(" << s.exp10() << ") M" << std::endl;
            }

            os << '}' << std::endl;
        }

    }

}

 
