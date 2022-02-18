#include "yack/chem/plexus.hpp"

namespace yack
{
    namespace chemical
    {


        static inline void add_nu(ios::ostream &fp, const size_t nu)
        {
            if(nu>1)
            {
                fp("[label=\"%lu\"]", static_cast<unsigned long>(nu));
            }
        }

        void plexus:: gv(const string &filename) const
        {
            //std::cerr << "plexus.gv(" << filename << ")" << std::endl;
            {
                ios::ocstream fp(filename);
                digraph_init(fp, "G");

                // write all species
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &sp = ***node;
                    sp.logo(fp) << '[';
                    sp.add_label(fp,sp.name());
                    fp << ",shape=box";
                    if(1==sp.rank)
                    {
                        fp << ",style=\"bold,dashed\"";
                    }
                    else
                    {
                        fp << ",style=bold";
                    }


                    end(fp << ']');
                }

                // write all equilibria
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    eq.logo(fp) << '[';
                    eq.add_label(fp,eq.name()) << ",style=filled";

                    switch(eq.type)
                    {
                        case equilibrium::is_unfinished: fp << ",shape=octagon"; break;
                        case equilibrium::has_both_ways: fp << ",shape=oval";    break;
                        case equilibrium::produces_only: fp << ",shape=trapezium";  break;
                        case equilibrium::consumes_only: fp << ",shape=invtrapezium";  break;

                    }
                    end(fp << ']');

                    // link
                    for(const actor *a=eq.reac.head;a;a=a->next)
                    {
                        const species &sp = **a;
                        sp.link(fp,&eq);
                        add_nu(fp,a->nu);
                        sp.end(fp);
                    }

                    for(const actor *a=eq.prod.head;a;a=a->next)
                    {
                        const species &sp = **a;
                        eq.link(fp,&sp);
                        add_nu(fp,a->nu);
                        sp.end(fp);
                    }



                }

                digraph_quit(fp);
            }

            ios::vizible::render(filename);
        }

    }
}

