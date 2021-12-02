#include "yack/jive/pattern/all.hpp"
#include "yack/ios/decoder.hpp"
#include "yack/exception.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {
        static const char fn[] = "jive::pattern::load ";

        pattern * pattern::load(ios::istream &fp)
        {
            uint32_t mark = 0;
            if(sizeof(mark)!=ios::decoder::read(fp,mark)) throw exception("%scannot read uuid",fn);

            switch(mark)
            {
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

                case counting::mark:
                {
                    size_t nmin = 0;
                    size_t nmax = 0;
                    (void) ios::decoder::construct(fp,nmin,"counting::nmin");
                    (void) ios::decoder::construct(fp,nmax,"counting::nmax");
                    return counting::create(nmin,nmax,load(fp));
                }


                default:
                    break;
            }

            throw exception("%sunhandled [%s]",fn,yack_fourcc(mark));

        }

    }

}

