#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline void notConserved(imatrix &NuA, const size_t j) throw()
        {
            for(size_t i=NuA.rows;i>0;--i) NuA[i][j] = 0;
        }

        static inline bool linkedRows(const readable<int> &lhs,
                                      const readable<int> &rhs) throw()
        {
            assert(lhs.size()==rhs.size());
            for(size_t j=lhs.size();j>0;--j)
            {
                if( lhs[j] && rhs[j] ) return true;
            }
            return false;
        }


        typedef meta_list<const equilibrium> ep_list;
        typedef ep_list::node_type           ep_node;

        class ep_group : public object, public ep_list
        {
        public:
            explicit ep_group() throw() : object(), ep_list(), next(0), prev(0) {}
            virtual ~ep_group() throw() {}

            bool linked_to(const equilibrium &eq, const imatrix &NuA) const throw()
            {
                assert(size>0);
                const readable<int> &lhs = NuA[*eq];
                //std::cerr << "testing " << eq.name << " @" << lhs << std::endl;
                for(const ep_node *en=head;en;en=en->next)
                {
                    const equilibrium   &me  = **en;
                    const readable<int> &rhs = NuA[*me];
                    //std::cerr << "against " << me.name << " @" << rhs;
                    if(linkedRows(lhs,rhs))
                    {
                        //std::cerr << " [+]" << std::endl;
                        return true;
                    }
                    //else std::cerr << " [-]" << std::endl;

                }

                return false;
            }

            ep_group *next;
            ep_group *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_group);
        };

        typedef cxx_list_of<ep_group> ep_groups_;

        class ep_groups : public ep_groups_
        {
        public:
            explicit ep_groups() throw() : ep_groups_() {}
            virtual ~ep_groups() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_groups);
        };


        void reactor:: conservation(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "Conservation");


            NuA.assign(Nu);
            vector<species *>     sdb(M,as_capacity);
            ep_list               edb;

            {
                addrbook          ebook;

                // Looping over species
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    const islot   &l = held_by[j];

                    if(verbose) corelib.pad(*xml<< s.name,s) << " : " << l << std::endl;
                    assert(s.rank==l.size);

                    bool    keep = true;
                    size_t  n    = 0;
                    size_t  p    = 0;

                    for(const enode *en=singles.head();en;en=en->next)
                    {
                        const equilibrium &eq = ***en;
                        const size_t       i  = *eq;
                        const feature      f  = eq.kind();

                        switch( __sign::of(Nu[i][j]) )
                        {
                            case __zero__:
                                continue;

                            case positive:
                                ++p;
                                break;

                            case negative:
                                ++n;
                                break;
                        }

                        YACK_XMLOG(xml,"|_[" << p << "+|" << n << "-]");

                        if(p>1||n>1)
                        {
                            // not a conservative equilibrium
                            keep = false;
                            YACK_XMLOG(xml,"|_shared");
                            notConserved(NuA,j);
                            goto DONE;
                        }

                        switch(f)
                        {
                            case undefined:
                                throw exception("undefined equilibrium <%s>", eq.name() );

                            case part_only:
                            case join_only:
                                // not a conservative equilibrium
                                keep = false;
                                YACK_XMLOG(xml,"|_excluded by " << eq.name);
                                notConserved(NuA,j);
                                goto DONE;

                            case both_ways:
                                break;
                        }

                        assert(both_ways==f);
                        ebook.ensure(&eq);
                    }

                DONE:
                    if(!keep) continue;
                    sdb << & coerce(s);
                }

                YACK_XMLOG(xml,"--> #species    = " << std::setw(6) <<  sdb.size()    << " / " << std::setw(6) << M);
                YACK_XMLOG(xml,"--> #equilibria = " << std::setw(6) << (*ebook).size  << " / " << std::setw(6) << N);

                for(addrbook::const_iterator it=ebook.begin();it!=ebook.end();++it)
                {
                    const void        *addr = *it; assert(NULL!=addr);
                    const equilibrium &eq   = *static_cast<const equilibrium *>(addr);
                    edb << & coerce(eq);
                }

            }



            std::cerr << "Nu  = " << Nu  << std::endl;
            std::cerr << "NuA = " << NuA << std::endl;


            singles.graphviz("eqs.dot",corelib);

            {
                YACK_XMLSUB(xml, "Grouping");

                ep_groups groups;


                while(edb.size)
                {
                    auto_ptr<ep_node>    en  = edb.pop_front();
                    const equilibrium   &eq  = **en;
                    YACK_XMLOG(xml, "-- hold <" << eq.name << "> ");
                    assert( both_ways == eq.kind() );
                    assert( iota::dot<int>::of(NuA[*eq],NuA[*eq])>0);

                    ep_group     *grp = NULL;
                    for(ep_group *g=groups.head;g;g=g->next)
                    {
                        if(g->linked_to(eq,NuA))
                        {
                            grp = g;
                            break;
                        }
                    }

                    if(!grp)
                    {
                        grp = new ep_group();
                        groups.push_back(grp);
                    }

                    assert(grp);
                    grp->push_back( en.yield() );
                }

                YACK_XMLOG(xml, "--> #group: " << groups.size);


            }


            exit(0);
            //"@eq1:-[a]+[b]:1"  "@eq2:-[b]+2[c]:1"  "@eq3:-[c]+[d]+[a]:1"
        }

    }

}

