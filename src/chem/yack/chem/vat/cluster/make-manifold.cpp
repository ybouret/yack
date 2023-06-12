#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/woven/qbuilder.hpp"
#include "yack/data/dinky/core-list.hpp"
#include "yack/type/abs.hpp"

namespace yack
{
    namespace Chemical
    {

        namespace
        {
            class icoeff : public object, public cxx_array<int>
            {
            public:
                inline explicit icoeff(const size_t n) : cxx_array<int>(n), next(0), prev(0) {}
                inline virtual ~icoeff() noexcept {}


                inline void load(const readable<int> &w) noexcept
                {
                    assert(w.size()==size());
                    for(size_t i=size();i>0;--i)
                    {
                        (*this)[i] = w[i];
                    }
                }

                

                icoeff *next, *prev;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(icoeff);
            };

            class icoeffs : public cxx_list_of<icoeff>
            {
            public:
                inline explicit icoeffs(const size_t n) noexcept : cxx_list_of<icoeff>(), dims(n) {}
                inline virtual ~icoeffs() noexcept {}

                bool insert(const readable<int> &w)
                {
                    assert(w.size()==dims);
                    for(const icoeff *node=head;node;node=node->next)
                    {
                        if(w == *node) return false;
                    }
                    
                    push_back( new icoeff(dims) )->load(w);
                    return true;
                }

                const size_t dims;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(icoeffs);
            };
        }


        typedef core_list<int>   iList;
        typedef iList::node_type iNode;

        static inline string MixedName(const Equilibrium::CoreRepo &mix,
                                       const iList                 &cof)
        {
            string                   id;
            const Equilibrium::Node *en = mix.head; assert(en);
            const iNode             *in = cof.head; assert(in);

            // first eq
            {
                const int nu = **in;
                switch(nu)
                {
                    case -1: id += '-'; break;
                    case  1: break;
                    default:
                        id += vformat("%d*",nu);
                }
                id += (***en).name;
            }

            // extra eq
            for(en=en->next,in=in->next;en;en=en->next,in=in->next)
            {
                assert(NULL!=in);
                const int nu = **in;
                switch(nu)
                {
                    case -1: id += '-'; break;
                    case  1: id += '+'; break;
                    default:
                        if(nu>0) id += '+';
                        id += vformat("%d*",nu);
                        break;
                }
                id += (***en).name;
            }

            return id;
        }


        class MixedEquilibrium : public Equilibrium
        {
        public:
            inline explicit MixedEquilibrium(const string           &user_uid,
                                             Equilibrium::CoreRepo  &user_mix,
                                             iList                  &user_cof,
                                             const Extended::Vector &user_usr) :
            Equilibrium(user_uid),
            mix(),
            cof(),
            usr(user_usr),
            neqz(0),
            norm(0)
            {
                coerce(mix).swap_with(user_mix);
                coerce(cof).swap_with(user_cof);
                assert(mix.size==cof.size);
            }

            inline virtual ~MixedEquilibrium() noexcept {}


            const Equilibrium::CoreRepo mix;
            const iList                 cof;
            const Extended::Vector     &usr;
            Extended::Mul               xmul;
            const size_t                neqz;
            const size_t                norm;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual Extended::Real getK(const double)
            {
                assert(mix.size>0);
                xmul.free();
                const Equilibrium::Node *en = mix.head;
                const iNode             *in = cof.head;
                for(size_t i=0;i>0;--i,en=en->next,in=in->next)
                {
                    assert(en);
                    assert(in);
                    const Equilibrium   &eq = ***en;
                    const size_t         ei = eq.indx[TopLevel];
                    if(ei<usr.size()) throw imported::exception(name(),"top-level constants not properly allocated");
                    const int            cf = **in;  assert(cf!=0);
                    const Extended::Real k  = usr[ eq.indx[TopLevel] ];

                }

                return 1;
                return xmul.reduce();
            }

        };

        typedef core_repo<MixedEquilibrium> MxList;
        typedef MxList::node_type           MxNode;

        static inline int MxCompare(const MxNode *lhs, const MxNode *rhs)
        {
            assert(lhs);
            assert(rhs);
            const MixedEquilibrium &L = ***lhs;
            const MixedEquilibrium &R = ***rhs;
            switch( __sign::of(L.neqz,R.neqz) )
            {
                case negative: return -1;
                case __zero__: break;
                case positive: return 1;
            }

            switch( __sign::of(L.norm,R.norm) )
            {
                case negative: return -1;
                case __zero__: break;
                case positive: return 1;
            }

            //return comparison::lexicographic(*lhs,*rhs);
            return 0;
        }


        void Cluster:: makeManifold(const xmlog      &xml,
                                    Equilibria       &eqs,
                                    Extended::Vector &usr)
        {
            const size_t nr = size;
            if(nr<=1) return;
            YACK_XMLSUB(xml,"Cluster::makeManifold");
            woven::zrepo     repo(nr);

            //------------------------------------------------------------------
            //
            // use WOVEn to build possible combinations
            //
            //------------------------------------------------------------------
            {
                matrix<int> Q(Nu,transposed);
                matrix<int> P;
                const size_t rank = woven::qcompress::build(P,Q);
                if(rank!=size)
                    throw imported::exception(CLID,"invalid compressed topology");


                woven::qbuilder  work(nr);
                work(repo,P,rank,false);
            }

            //------------------------------------------------------------------
            //
            // study possible combinations
            //
            //------------------------------------------------------------------
            const size_t   nc = lib.size;
            icoeffs        idb(nc);
            cxx_array<int> sto(nc);
            addrbook       adb;
            MxList         sub;
            for(const woven::zvector *v=repo.head;v;v=v->next)
            {

                //--------------------------------------------------------------
                // compute coefficients of the new equilibrium
                //--------------------------------------------------------------
                const readable<apz>  &weight = *v;
                Equilibrium::CoreRepo mix;
                iList                 cof;
                adb.free();
                sto.ld(0);
                for(const Equilibrium::Node *node=head;node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    const size_t       ei = eq.indx[SubLevel];
                    const int          ew = weight[ei].cast_to<int>("weight");
                    if(0==ew) continue;
                    eq.submitTo(adb);
                    mix << eq;
                    cof << ew;
                    for(size_t j=nc;j>0;--j)
                    {
                        sto[j] += ew * Nu[ei][j];
                    }
                }
                assert(cof.size==mix.size);

                //--------------------------------------------------------------
                // combination of at least two equilibria
                //--------------------------------------------------------------
                if(mix.size<2)
                {
                    //YACK_XMLOG(xml," (-) no mix @" << weight);
                    continue;
                }


                //--------------------------------------------------------------
                // combination must remove some species
                //--------------------------------------------------------------
                const size_t inp = adb->size;
                const size_t out = woven::qcompress::neqz(sto);
                if(out>=inp)
                {
                    //YACK_XMLOG(xml," (-) no vanishing species @" << weight);
                    continue;
                }

                if(!idb.insert(sto))
                {
                    // YACK_XMLOG(xml," (-) multiple mix @" << sto);
                    continue;
                }

                //--------------------------------------------------------------
                // construct the now VALID mixed equilibrium
                //--------------------------------------------------------------
                const string      mixedName = MixedName(mix,cof);
                MixedEquilibrium &mixed     = eqs( new MixedEquilibrium(mixedName,mix,cof,usr) );
                for(const Species::Node *sn=lib.head;sn;sn=sn->next)
                {
                    const Species &sp = ***sn;
                    const size_t   sj = sp.indx[SubLevel];
                    const int      nu = sto[sj];
                    if(nu!=0)
                    {
                        mixed(nu,sp);
                        coerce(mixed.neqz)++;
                        coerce(mixed.norm) += absolute(nu);
                    }
                }
                sub << mixed;
            }

            //------------------------------------------------------------------
            //
            // merge ordered combinations
            //
            //------------------------------------------------------------------
            {
                merge_list_of<MxNode>::sort(sub, MxCompare);
                size_t idx = (***tail).indx[SubLevel];
                while(sub.size)
                {
                    auto_ptr<MxNode> node = sub.pop_front();
                    Equilibrium     &eq   = ***node;
                    *this << eq;
                    update(eq);
                    coerce(eq.indx[SubLevel]) = ++idx;
                }
            }

            {
                coerce(reac.maxlen) = 0;
                coerce(prod.maxlen) = 0;

                for(const Equilibrium::Node *node=head;node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    reac.update(eq.reac);
                    prod.update(eq.prod);
                }

            }

            std::cerr << *this << std::endl;

        }

    }

}


