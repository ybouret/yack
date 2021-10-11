
#include "yack/memory/parcel.hpp"

namespace yack
{

    namespace memory
    {

        parcel:: ~parcel() throw()
        {
        }

        parcel:: parcel(void        *block_addr,
                        const size_t block_size,
                        const size_t block_exp2 ) throw():
        biggest_stamp(static_cast<stamp_t *>(block_addr)),
        usable_stamps(1),
        head(biggest_stamp),
        tail( head+(block_size/stamp_size)-1 ),
        next(0),
        prev(0),
        data_size(block_size),
        data_exp2(block_exp2)
        {
            assert(data_size>=min_data_size);
            assert(data_size==size_t(1)<<data_exp2);
            
            head->next=tail;
            head->prev=0;
            head->used=0;
            head->size=(static_cast<size_t>(tail-head)-1) << stamp_exp2;

            tail->prev=head;
            tail->next=0;
            tail->used=1;
            tail->size=0;
            
        }



    }

}

#include <iostream>

namespace yack
{

    namespace memory
    {

        void parcel:: display() const
        {
            std::cerr << '[';
            for(const stamp_t *stamp=head;stamp!=tail;stamp=stamp->next)
            {
                if(stamp->used)
                {
                    std::cerr << '-';
                }
                else
                {
                    std::cerr << '+';
                }
                std::cerr << stamp->size;
            }
            std::cerr << ']' << '#' << data_size << std::endl;
        }

    }

}
