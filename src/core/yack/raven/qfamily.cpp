#include "yack/raven/qfamily.hpp"
#include "yack/system/imported.hpp"


namespace yack
{
    namespace raven
    {

        qfamily:: ~qfamily() throw()
        {
        }

        void qfamily:: throw_singular_matrix(const size_t ir) const
        {
            throw imported::exception( "raven::qfamily","singular matrix[%u]", unsigned(ir) );
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase) << "@" << *(self.basis) << "+" << *(self.ready);
            return os;
        }


        qfamily:: qfamily(const qfamily &F) :
        object(),
        qbase( F.qbase ),
        basis( F.basis ),
        ready( F.ready ),
        next(0),
        prev(0)
        {
        }


        void qfamily:: reduce(list_of<qfamily> &lineage)
        {
            cxx_list_of<qfamily> repo;

            while( lineage.size )
            {
                auto_ptr<qfamily> tribeF = lineage.pop_front();
                const qmatrix    &tribeQ = **tribeF;
                if(tribeQ.active_state == fully_grown)
                {
                    std::cerr << "fully grown" << std::endl;
                    continue;
                }
                bool              housed = false;

                std::cerr << " => " << tribeQ << std::endl;

                for(qfamily *houseF=repo.head;houseF;houseF=houseF->next)
                {
                    qmatrix &houseQ = **houseF;
                    std::cerr << " |_" << houseQ << std::endl;
                    if(houseQ.is_equivalent_to(tribeQ)) {
                        std::cerr << "Equivalent " << tribeQ << " and " << houseQ << " !" << std::endl;
                        housed = true;
                    }
                }


                repo.push_back( tribeF.yield() );
                std::cerr << "\t#repo=" << repo.size << std::endl;
            }

            lineage.swap_with(repo);

        }

    }

}

