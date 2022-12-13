#include "yack/apex/north/qfamily.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace north
    {
        qfamily:: ~qfamily() throw()
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
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase)
            << "@" << *(self.basis)
            << "+" << *(self.ready);
            return os;
        }

    }
}
