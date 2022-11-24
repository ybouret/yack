
#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/boolean.h"
#include "yack/type/utils.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/sort/indexing.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        //----------------------------------------------------------------------
        //
        // counting coefficients != 0
        //
        //----------------------------------------------------------------------
        template <typename T>
        static inline size_t get_coeff_count(const readable<T> &nu) throw()
        {
            size_t     count = 0;
            for(size_t j=nu.size();j>0;--j)
            {
                if(nu[j]!=0) ++count;
            }
            return count;
        }

        //----------------------------------------------------------------------
        //
        // algebra orthogonal family, simplified to co-prime integers
        //
        //----------------------------------------------------------------------
        static const  char fn[] = "conservation";
        static inline void create_ortho_family(matrix<apq>       &Q,
                                               const matrix<apq> &P)
        {
            assert(P.cols==Q.cols);
            assert(Q.rows==Q.cols);
            const size_t N  = P.rows;
            const size_t M  = P.cols;
            const apq    _1 = 1;

            matrix<apq> Pt(P,transposed);
            matrix<apq> P2(N,N); iota::gram(P2,P);
            crout<apq>  lu(N);   if(!lu.build(P2)) throw exception("%s: invalid matrix rank!!",fn);
            matrix<apq> rhs(P);
            lu.solve(P2,rhs);
            iota::mmul(Q,Pt,rhs);
            for(size_t i=M;i>0;--i)
            {
                writable<apq> &Qi = Q[i];
                for(size_t j=M;j>i;--j)
                {
                    Qi[j] = -Qi[j];
                }
                Qi[i] = _1 - Qi[i];
                for(size_t j=i-1;j>0;--j)
                {
                    Qi[j] = -Qi[j];
                }
                apk::simplify(Qi);
            }
        }


        //----------------------------------------------------------------------
        //
        // test that all coefficients are >= 0
        //
        //----------------------------------------------------------------------
        template <typename T>
        static inline bool are_all_geqz(const readable<T> &arr)
        {
            for(size_t i=arr.size();i>0;--i)
            {
                if(arr[i]<0) return false;
            }

            return true;
        }

        //----------------------------------------------------------------------
        //
        // vector of apz
        //
        //----------------------------------------------------------------------
        class zvector : public object, public counted, public vector<apz>
        {
        public:
            explicit zvector(const size_t M) : object(), counted(), vector<apz>(M)
            {
                assert(size()==M);
            }

            virtual ~zvector() throw()
            {
            }

            zvector(const zvector &_) : object(), counted(), vector<apz>(_)
            {
            }

            explicit zvector(const readable<apq> &q) : object(), counted(), vector<apz>(q.size())
            {
                for(size_t i=q.size();i>0;--i)
                {
                    (*this)[i] = q[i].num; assert(1==q[i].den);
                }
            }


        private:
            YACK_DISABLE_ASSIGN(zvector);
        };


        typedef arc_ptr<zvector> zcoeffs;
        typedef vector<zcoeffs>  zcArray;

        //----------------------------------------------------------------------
        //
        // create local repository
        // of all possible positive sparse combinations
        //
        //----------------------------------------------------------------------
        static inline
        void create_positive(zcArray             &arr,
                             const readable<apz> &lhs,
                             const readable<apz> &rhs)
        {
            static const  unsigned has_pos = 0x01;
            static const  unsigned has_neg = 0x02;
            static const  unsigned has_mix = has_pos | has_neg;

            assert( lhs.size() == rhs.size() );
            const size_t M = lhs.size();
            zvector      vtry(M);

            //------------------------------------------------------------------
            // loop over coefficients
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const apz &l = lhs[j];
                const apz &r = rhs[j];
                if(0==l||0==r) continue; // no combination

                const apz rw   = -l;   // right weight
                const apz lw   = r;    // left  weight
                size_t    np   = 0;    // |positive|
                size_t    nn   = 0;    // |negative|
                unsigned  flag = 0x00; // status

                //--------------------------------------------------------------
                // create combination
                //--------------------------------------------------------------
                for(size_t k=M;k>0;--k)
                {
                    switch( (vtry[k] = lw * lhs[k] + rw * rhs[k]).s )
                    {
                        case __zero__:
                            break;

                        case positive:
                            flag |= has_pos;
                            ++np;
                            break;

                        case negative:
                            flag |= has_neg;
                            ++nn;
                            break;
                    }
                }

                //--------------------------------------------------------------
                // study result
                //--------------------------------------------------------------
                switch(flag)
                {
                    case has_mix: // mixed
                        assert(nn>0);
                        assert(np>0);
                        continue;

                    case has_pos:
                        assert(nn<=0);
                        if(np>1)
                        {
                            const zcoeffs zc = new zvector(vtry);
                            arr.push_back(zc);
                            apk::simplify(*arr.back());
                        }
                        break;

                    case has_neg:
                        assert(np<=0);
                        if(nn>1)
                        {
                            iota::neg(vtry);
                            const zcoeffs zc = new zvector(vtry);
                            arr.push_back(zc);
                            apk::simplify(*arr.back());
                        }
                        break;


                    default:
                        assert(!flag);
                        continue;
                }

            }



        }


        //----------------------------------------------------------------------
        //
        // database of coefficients
        //
        //----------------------------------------------------------------------
        class zcdbase : public zcArray
        {
        public:
            explicit zcdbase() throw() : zcArray() {}
            virtual ~zcdbase() throw() {}

            void grow(const zvector &zvec)
            {
                // test all combinations with previously inserted
                zcArray arr;
                for(size_t i=size();i>0;--i)
                {
                    arr.free();
                    create_positive(arr,*(*this)[i],zvec);
                    while(arr.size())
                    {
                        grow1( *arr.back() );
                        arr.pop_back();
                    }
                }

                // and zvec only
                if(are_all_geqz(zvec))
                    grow1(zvec);
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zcdbase);
            void grow1(const zvector &zvec)
            {

                for(size_t i=size();i>0;--i)
                {
                    const zvector &self = *(*this)[i];
                    if( apk::are_prop(zvec,self,NULL))
                    {
                        // reject if proportional to existing
                        return;
                    }
                }

                // no proportionality => add
                const zcoeffs zc = new zvector(zvec);
                push_back(zc);
            }
        };



        //----------------------------------------------------------------------
        //
        // initial dispatch of possible combinations
        //
        //----------------------------------------------------------------------
        static inline
        void dispatch(zcArray           &zpos,
                      zcArray           &zneg,
                      const matrix<apq> &Q,
                      const xmlog       &xml)
        {
            for(size_t i=Q.rows;i>0;--i)
            {
                const zcoeffs zc = new zvector(Q[i]);
                if(get_coeff_count(*zc) <= 1) continue;
                if(are_all_geqz(*zc))
                {
                    YACK_XMLOG(xml, "[+] " << zc);
                    zpos.push_back(zc);
                }
                else
                {
                    YACK_XMLOG(xml, "[-] " << zc);
                    zneg.push_back(zc);
                }
            }
        }

        //----------------------------------------------------------------------
        //
        // weight is norm1 of (positive!) coefficients
        //
        //----------------------------------------------------------------------
        static inline
        apn weight_of(const readable<apz> &z)
        {
            apn sum = 0;
            for(size_t i=z.size();i>0;--i)
            {
                assert(z[i]>=0);
                sum += z[i].n;
            }
            return sum;
        }


        //----------------------------------------------------------------------
        //
        // find all sparse conservations
        //
        //----------------------------------------------------------------------
        void look_up_conservations(matrix<unsigned>  &F,
                                   const matrix<apq> &Q,
                                   const xmlog       &xml)
        {
            YACK_XMLSUB(xml,"lookUp");
            assert(F.rows==0);
            assert(F.cols==0);

            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- initialize coefficients");
            //------------------------------------------------------------------
            zcArray zpos;
            zcArray zneg;
            dispatch(zpos,zneg,Q,xml);

            zcdbase db;
            if(zpos.size()) {
                YACK_XMLOG(xml, "-- initializing database with " << zpos.back() );
                db.push_back( zpos.back() ); zpos.pop_back();
            }

            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- flushing existing positive" );
            //------------------------------------------------------------------
            while( zpos.size() > 0)
            {
                db.grow(*zpos.back());
                zpos.pop_back();
            }

            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- flushing existing negative" );
            //------------------------------------------------------------------
            const size_t nn = zneg.size();
            for(size_t i=1;i<=nn;++i)
            {
                const zvector &lhs = *zneg[i];
                db.grow(lhs);

                for(size_t j=i+1;j<=nn;++j)
                {
                    const zvector &rhs = *zneg[j];
                    zcArray        arr;
                    create_positive(arr,lhs,rhs);
                    while(arr.size())
                    {
                        db.grow( *arr.back() );
                        arr.pop_back();
                    }
                }
            }

            YACK_XMLOG(xml, "-- |positive| = " << db.size() );

            //------------------------------------------------------------------
            // computing weight of each combination
            //------------------------------------------------------------------
            const size_t n = db.size();
            vector<apn>  w(n);
            for(size_t i=1;i<=n;++i)
            {
                w[i] = weight_of(*db[i]);
            }

            //------------------------------------------------------------------
            // indexing
            //------------------------------------------------------------------
            vector<size_t> indx(n);
            indexing::make(indx, comparison::increasing<apn>, w);

            //------------------------------------------------------------------
            // computing rank
            //------------------------------------------------------------------
            size_t rank = 0;
            {
                matrix<apq>    W(n,Q.cols);
                for(size_t i=1;i<=n;++i)
                {
                    const size_t ii = indx[i];
                    if(xml.verbose) std::cerr << "\tW" << i << "\t= " << db[ii] << "  // " << w[ii] << std::endl;;
                    iota::load(W[i],*db[ii]);
                }
                rank = apk::gj_rank(W);
            }
            YACK_XMLOG(xml, "-- |rank|     = " << rank);

            if(rank)
            {
                //--------------------------------------------------------------
                // extract a maximal matrix with smallest weights
                //--------------------------------------------------------------
                const size_t M = Q.cols;
                F.make(rank,M);
                size_t i=0;
                size_t k=0;

            NEXT_I:
                ++i;
                writable<unsigned>  &Fi = F[i];
            NEXT_K:
                ++k;
                const readable<apz>     &Wk   = *db[ indx[k] ];
                for(size_t j=M;j>0;--j) Fi[j] = Wk[j].cast_to<unsigned>();
                if( apk::gj_rank_of(F) < i )
                    goto NEXT_K;
                if(i<rank)
                    goto NEXT_I;
            }

        }



        void reactor::conservation2(const xmlog &xml)
        {
            YACK_XMLSUB(xml,fn);


            //------------------------------------------------------------------
            //
            //
            // initialize NuA to Nu
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- removing unconserved species" );
            NuA.assign(Nu);
            for(const enode *en=singles.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;

                switch( eq.kind() )
                {
                        // undefined is bad at this point...
                    case undefined: throw exception("%s: undefined <%s>", fn, eq.name() );

                        // remove all involved species
                    case part_only:
                    case join_only:
                        for(const cnode *cn=eq.head();cn;cn=cn->next)
                        {
                            const species &sp = ****cn;
                            const size_t   sj = *sp;
                            for(size_t i=N;i>0;--i) NuA[i][sj] = 0;
                        }
                        break;

                        // ok
                    case both_ways:
                        break;
                }
            }

            if(verbose) std::cerr << "NuA=" << NuA << std::endl;

            //------------------------------------------------------------------
            //
            //
            // counting remaining dimensions
            //
            //
            //------------------------------------------------------------------
            matrix<apq> P;
            {

                //--------------------------------------------------------------
                // detecting remaining equilibria
                //--------------------------------------------------------------
                meta_list<const equilibrium> eqdb;
                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium   &eq = ***en;
                    switch(get_coeff_count(NuA[*eq]))
                    {
                        case 0:
                            break;

                        case 1:
                            throw exception("%s: unexpected remaining 1 species in <%s>",fn,eq.name());

                        default:
                            eqdb << &eq;
                    }
                }

                //--------------------------------------------------------------
                // check validity
                //--------------------------------------------------------------
                if(eqdb.size<=0) {
                    YACK_XMLOG(xml, "-- no conservation");
                    return;
                }

                //--------------------------------------------------------------
                // create matrix
                //--------------------------------------------------------------
                P.make(eqdb.size,M);
                size_t i=1;
                for(const meta_node<const equilibrium> *en=eqdb.head;en;en=en->next,++i)
                {
                    iota::load(P[i],NuA[***en]);
                }
            }
            const size_t Nc = P.rows;
            YACK_XMLOG(xml, "-- look up against Nc=" << Nc);
            if(verbose) std::cerr << "\tP=" << P << std::endl;

            //------------------------------------------------------------------
            //
            //
            // create orthogonal family
            //
            //
            //------------------------------------------------------------------
            matrix<apq> Q(M,M);
            create_ortho_family(Q,P);
            if(verbose) std::cerr << "\tQ=" << Q << std::endl;

            //------------------------------------------------------------------
            //
            //
            // start parsity
            //
            //
            //------------------------------------------------------------------
            matrix<unsigned> F;
            look_up_conservations(F,Q,xml);
            std::cerr << "F=" << F << std::endl;
            std::cerr << "Nu=" << Nu << std::endl;

            //------------------------------------------------------------------
            //
            //
            // convert to human readable constraints
            //
            //
            //------------------------------------------------------------------
            const size_t nc = F.rows;
            for(size_t i=1;i<=nc;++i)
            {
                constraint A = new conserve();
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s  = **an;
                    const unsigned w = F[i][*s];
                    if(w)
                        (*A)(s,w);
                }

                std::cerr << "constraint: d(" << A << ")=0" << std::endl;
                cnsv.push_back(A);
            }


            // "@eq:-[a]+4[b]+7[c]-2[d]:1"
        }


    }

}


