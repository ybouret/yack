#include "yack/apex/north/qfamily.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace north
    {
        qfamily:: ~qfamily() throw()
        {
        }

        qfamily:: qfamily(const qfamily &origin) :
        qbase(origin.qbase),
        basis(origin.basis),
        ready(origin.ready),
        next(0),
        prev(0)
        {
        }

        const char qfamily::clid[] = "north::qfamily";

        void qfamily:: throw_invalid_init(const size_t ir)
        {
            throw imported::exception(clid,"invalid initial vbase[%u]",unsigned(ir));
        }

        void qfamily:: assign_all_indices(const readable<size_t> &rindx)
        {
            const size_t nr = rindx.size(); assert(rindx.size()>0);
            const size_t ir = rindx[nr];    assert(ir>0);
            basis << ir;
            for(size_t i=1;i<nr;++i) ready << rindx[i];
            assert(basis.excludes(ready));
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase)
            << "@" << *(self.basis)
            << "+" << *(self.ready);
            return os;
        }


        void qfamily:: reduce(list_of<qfamily> &lineage)
        {
            std::cerr << "reducing" << std::endl;
            list_type result;
            while(lineage.size)
            {
                auto_ptr<qfamily> tribe = lineage.pop_front();
                bool              enjoy = true;
                const qmatrix    &lhs   = *(tribe->qbase);
                for(qfamily      *house = result.head;house;house=house->next)
                {
                    const qmatrix &rhs = *(house->qbase);
                    if(lhs==rhs)
                    {
                        std::cerr << "found multiple " << lhs << std::endl;
                        enjoy = false;

                        house->basis += tribe->basis;
                        house->ready += tribe->ready;
                        house->ready -= house->basis;

                    }
                }

                if(enjoy)
                    result.push_back( tribe.yield() );
            }
            result.swap_with(lineage);
        }
        


    }
}
