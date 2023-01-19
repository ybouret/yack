
#include "yack/chem/reactor/equilibrium/squads.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {

    namespace chemical {

        squads:: ~squads() throw()
        {
        }

        squads:: squads() throw() : object() {}


        void squads:: shape(const eq_repo_     &assembly,
                            const matrix<bool> &detached)
        {

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            for(const eq_node *en=assembly.head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                push_back( new squad(eq) );
            }

            //------------------------------------------------------------------
            //
            // try to grow with each member of the assembly
            //
            //------------------------------------------------------------------
            for(const eq_node *en=assembly.head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                for(squad *sq=head;sq;sq=sq->next)
                {
                    if( sq->accepts(eq,detached) )
                    {
                        (*push_front( sq->clone() )) << eq;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // sort
            //
            //------------------------------------------------------------------
            merge_list_of<squad>::sort(*this,squad::compare);

            //------------------------------------------------------------------
            //
            // compile
            //
            //------------------------------------------------------------------
            for(squad *sq=head;sq;sq=sq->next) sq->compile();
        }

        void squads:: print(const xmlog &xml, const char * const info) const
        {
            if(xml.verbose)
            {
                YACK_XMLSUB(xml,info);
                for(const squad *sq = head;sq;sq=sq->next)
                {
                    *xml << *sq << std::endl;
                }
            }

        }

    }

}

