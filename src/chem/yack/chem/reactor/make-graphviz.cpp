#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {

        static inline void sp_viz(ios::ostream  &fp,
                                  const species &s,
                                  const char    *attr)
        {
            s.logo(fp) << '[';
            s.add_label(fp,s.name());
            fp << attr;
            s.end(fp << ']');
        }

        static inline void eq_viz(ios::ostream &fp,
                                  const equilibrium &eq,
                                  const char *attr)
        {

            eq.logo(fp) << '[';
            eq.add_label(fp,eq.name());
            fp << attr;
            eq.end(fp << ']');

            for(const actor *a = eq.reac->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                s.link(fp,&eq);
                if(nu>1) fp("[label=\"%u\"]",nu);
                s.end(fp);
            }

            for(const actor *a = eq.prod->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                eq.link(fp,&s);
                if(nu>1) fp("[label=\"%u\"]",nu);
                eq.end(fp);
            }
        }



        void cluster:: viz(ios::ostream &fp) const
        {
            fp << "subgraph cluster_";
            fp("%u",gvidx);
            fp << " {\n";
            fp << "style=bold;\n";

            // write all species
            for(const sp_gnode *sn=breed->conserved->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=egg,style=filled");
            }

            for(const sp_gnode *sn=breed->unbounded->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=egg");
            }


            // write all equilibria
            size_t count=0;
            for(const eq_gnode *en=genus->delimited->head;en && (++count<=genus->delimited->core);en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=box,style=bold");
            }

            count = 0;
            for(const eq_gnode *en=genus->reac_only->head;en && (++count<=genus->reac_only->core);en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=house");
            }

            count = 0;
            for(const eq_gnode *en=genus->prod_only->head;en && (++count<=genus->prod_only->core);en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=invhouse");
            }

            // write conservation laws
            for(const claw *cl=canon->head;cl;cl=cl->next)
            {
                cl->viz(fp);
            }

            fp << "}\n";
        }

    }

}

#include "yack/chem/reactor.hpp"

namespace yack
{
    namespace chemical
    {
        void reactor:: viz_obs(ios::ostream &fp) const
        {
            const sp_node *node = obs->head;
            if(node)
            {
                const unsigned idx = linked->tail ? linked->tail->gvidx + 1 : 0;
                fp << "subgraph cluster_";
                fp("%u",idx);
                fp << " {\n";
                fp << "style=\"bold,dashed\";\n";
                for(;node;node=node->next) {
                    const species &s = ***node;
                    sp_viz(fp,s, ", shape=rectangle,style=dashed");
                }
                fp << " }\n";
            }
        }


        void eq_repo:: viz(ios::ostream            &fp,
                           const sub_list<species> &act,
                           const readable<bool>    &flg,
                           const unsigned int       igv) const
        {
            fp << "subgraph cluster_";
            fp("%u",igv);
            fp << " {\n";
            fp << "style=bold;\n";

            // write species
            for(sub_list<species>::node_type *node=act->head;node;node=node->next)
            {
                const species &sp = node->host;
                const char    *id = flg[ **node ] ? ", shape=egg,style=filled" : ", shape=egg";
                sp_viz(fp,sp,id);
            }

            // write equilibria
            size_t count=0;
            for(const eq_node *node = head; node; node=node->next)
            {
                const unsigned c = unsigned( 1 + (count++ % 7) ); assert(c>=1); assert(c<=7);
                const string color = vformat("color=\"/dark27/%u\"",c);
                fp << " node  [" << color << "];\n";
                fp << " edge  [" << color << ", font" << color << "];\n";
                
                const equilibrium &eq = ***node;
                eq_viz(fp,eq,",shape=box,style=bold");
            }

            fp << " }\n";
        }

    }

}


#include "yack/fs/local/fs.hpp"
#include "yack/jive/pattern/matching.hpp"

namespace yack
{
    namespace chemical
    {

        static inline void remove_png(const string &root,
                                      vfs          &fs)
        {
            vector<string>         path;
            {
                const string           expr  = root + "[:digit:]*" + "\\.png";
                jive::matching         match = expr;
                const string           here  = vfs::get_dir_name(root);
                auto_ptr<vfs::scanner> scan  = fs.open_folder(here);
                vfs::entry            *ep    = 0;
                while(NULL!=(ep=scan->next()))
                {
                    auto_ptr<vfs::entry> keep(ep);
                    if(!ep->is_reg()) continue;
                    if(match.exactly(ep->base)) path << *(ep->path);
                }
            }
            //std::cerr << "bad=" << path << std::endl;
            for(size_t i=path.size();i>0;--i) fs.try_remove_file(path[i]);
        }

        void reactor:: graphViz(const string &root) const
        {
            static localFS &fs = localFS::instance();

            // core system
            {
                const string fn = root + ".dot";
                fs.try_remove_file(fn);
                remove_png(root,fs);
                {
                    ios::ocstream fp(fn);
                    ios::vizible::digraph_init(fp,"G");
                    for(const cluster *cl=linked->head;cl;cl=cl->next)
                    {
                        cl->viz(fp);
                    }
                    viz_obs(fp);
                    ios::vizible::digraph_quit(fp);
                }
                ios::vizible::render(fn);
            }

            // cross system
            const size_t nd = max_degree();
            for(size_t i=2;i<=nd;++i)
            {
                const string fn = root + vformat("%u",unsigned(i)) + ".dot";
                {
                    ios::ocstream fp(fn);
                    ios::vizible::digraph_init(fp,"G");
                    for(const cluster *cl=linked->head;cl;cl=cl->next)
                    {
                        const ledger &gvec = *(cl->cross);
                        if(gvec.size()>=i) {
                            gvec[i]->viz(fp,*(cl->alive),*(cl->fixed),cl->gvidx);
                        }
                    }
                    ios::vizible::digraph_quit(fp);
                }
                ios::vizible::render(fn);
            }
        }

    }
}

