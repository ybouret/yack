#include "yack/jive/pattern/all.hpp"
#include "yack/ios/decoder.hpp"
#include "yack/exception.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {
        static const char fn[] = "jive::pattern::load ";


        template <typename LOGICAL> static inline
        pattern *load_op(ios::istream &fp)
        {
            auto_ptr<LOGICAL> p = new LOGICAL();
            size_t            n = 0;
            (void) ios::decoder::construct(fp, n, "#operands");
            while(n-- > 0)
            {
                (*p) << pattern::load(fp);
            }
            return p.yield();
        }

        pattern * pattern::load(ios::istream &fp)
        {
            uint32_t mark = 0;
            if(sizeof(mark)!=ios::decoder::read(fp,mark)) throw exception("%scannot read uuid",fn);

            switch(mark)
            {
                    //----------------------------------------------------------
                    //
                    // basic
                    //
                    //----------------------------------------------------------
                case single::mark:
                {
                    char C = 0; if(!fp.query(C)) throw exception("%smissing code for %s",fn,single::clid);
                    return new single(C);
                }

                case within::mark:
                {
                    char lower = 0; if(!fp.query(lower)) throw exception("%smissing lower for %s",fn,within::clid);
                    char upper = 0; if(!fp.query(upper)) throw exception("%smissing upper for %s",fn,within::clid);
                    return new within(lower,upper);
                }

                case except::mark:
                {
                    char C = 0; if(!fp.query(C)) throw exception("%smissing code for %s",fn,except::clid);
                    return new except(C);
                }

                    //----------------------------------------------------------
                    //
                    // joker
                    //
                    //----------------------------------------------------------
                case optional::mark:
                {
                    return optional::create( load(fp) );
                }

                case at_least::mark:
                {
                    size_t count = 0;
                    (void) ios::decoder::construct(fp,count,"at_least::count");
                    return at_least::create(count,load(fp));
                }

                case at_least::zom_: return zero_or_more(load(fp));
                case at_least::oom_: return one_or_more(load(fp));

                case counting::mark:
                {
                    size_t nmin = 0;
                    size_t nmax = 0;
                    (void) ios::decoder::construct(fp,nmin,"counting::nmin");
                    (void) ios::decoder::construct(fp,nmax,"counting::nmax");
                    return counting::create(nmin,nmax,load(fp));
                }
                    //----------------------------------------------------------
                    //
                    // logical
                    //
                    //----------------------------------------------------------
                case op_and:: mark:  return load_op<op_and>(fp);
                case op_or::  mark:  return load_op<op_or>(fp);
                case op_none::mark:  return load_op<op_none>(fp);

                default:
                    break;
            }

            throw exception("%sunhandled [%s]",fn,yack_fourcc(mark));

        }

    }

}

