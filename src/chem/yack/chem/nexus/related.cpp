
#include "yack/chem/nexus.hpp"

namespace yack
{
    namespace chemical
    {

        void nexus:: build_related(const xmlog &xml)
        {
            static const char * const fn = "build_related";
            YACK_XMLSUB(xml,fn);

            //------------------------------------------------------------------
            //
            // build algorithm
            //
            //------------------------------------------------------------------
            {
                //YACK_XMLSUB(xml,"building");
                clusters &cls = coerce(related); assert(cls.size<=0);

                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    YACK_XMLOG(xml, "<" << eq.name << ">");

                    //----------------------------------------------------------
                    // look for an attach
                    //----------------------------------------------------------
                    cluster *curr = cls.head;
                    while(curr)
                    {
                        if(curr->attached_to(eq))
                        {
                            YACK_XMLOG(xml,"\tattached to "<< *curr);
                            break;
                        }
                        curr = curr->next;
                    }

                    if(curr)
                    {
                        //------------------------------------------------------
                        // attach equilibrium to existing cluster
                        //------------------------------------------------------
                        (*cls.pop(curr)) << &eq;

                        //------------------------------------------------------
                        // reduction if the new cluster encompasses old ones
                        //------------------------------------------------------
                        {
                            cluster &rhs = *curr;
                            clusters tmp;
                            while(cls.size)
                            {
                                cluster *lhs = cls.pop_front();
                                if(lhs->attached_to(rhs))
                                {
                                    YACK_XMLOG(xml, "\tmerged with " << *lhs);
                                    rhs.merge_back(*lhs);
                                    delete lhs;
                                }
                                else
                                {
                                    tmp.push_back(lhs);
                                }
                            }
                            cls.swap_with(tmp);
                        }

                        cls.push_back(curr);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // start new cluster
                        //------------------------------------------------------
                        (*cls.push_back( new cluster() )) << &eq;
                        YACK_XMLOG(xml, "\tcreated new " << *cls.tail );
                    }
                }
                
                for(cluster *cc=cls.head;cc;cc=cc->next)
                {
                    cc->sort();
                }
            }

            YACK_XMLOG(xml,"related = " << related);

            
        }

    }


}

