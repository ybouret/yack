#include "yack/container/matrix/metrics.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"

#include <iostream>

namespace yack
{

    const matrix_metrics::transposed_t transposed = {};

    typedef memory::pooled matrix_allocator;

    matrix_metrics:: ~matrix_metrics() throw()
    {
        deallocate();
    }
    
    void matrix_metrics:: deallocate() throw()
    {
        if(rows)
        {
            static memory::allocator   &mem = matrix_allocator::location();
            mem.release(workspace,coerce(allocated));
            coerce(items) = 0;
            coerce(cols)  = 0;
            coerce(rows)  = 0;
        }
    }
    
    
    matrix_metrics:: matrix_metrics(  ) throw():
    rows(0),
    cols(0),
    items(0),
    stride(0),
    allocated(0),
    workspace(0)
    {
    }

    void matrix_metrics:: swap_with(matrix_metrics &other) throw()
    {
        coerce_cswap(rows,other.rows);
        coerce_cswap(cols,other.cols);
        coerce_cswap(items,other.items);
        coerce_cswap(allocated,other.allocated);
        coerce_cswap(workspace,other.workspace);

    }

    
    matrix_metrics:: matrix_metrics(void **       row_hook,
                                    void **       ptr_hook,
                                    const size_t  num_rows,
                                    const size_t  num_cols,
                                    const size_t  size_of_item,
                                    row_callback  build_row_at) :
    rows(num_rows),
    cols(num_cols),
    items(rows*cols),
    stride(cols*size_of_item),
    allocated(0),
    workspace(0)
    {
        assert(build_row_at);
        assert(row_hook);
        assert(ptr_hook);

        // sanity check
        assert(size_of_item>0);
        if(rows<=0)
        {
            if(cols>0)  throw libc::exception(EDOM,"matrix(rows=0,cols=%u)",unsigned(cols));
        }
        else
        {
            assert(rows>0);
            if(cols<=0) throw libc::exception(EDOM,"matrix(rows=%u,cols=9)",unsigned(rows));
        }
        
        if(rows>0)
        {
            typedef matrix_row<uint8_t> row_t;
            static  memory::allocator   &mem = matrix_allocator::instance();
            
            //__________________________________________________________________
            //
            // allocate
            //__________________________________________________________________
            row_t               *temp = 0;
            uint8_t             *data = 0;
            memory::embed        emb[] =
            {
                memory::embed(temp,rows),
                memory::embed(data,items*size_of_item)
            };
            workspace = YACK_MEMORY_EMBED(emb,mem,coerce(allocated));

            //__________________________________________________________________
            //
            // link
            //__________________________________________________________________
            *row_hook = out_of_reach::address(temp-1);
            *ptr_hook = out_of_reach::address(data);
            {
                uint8_t *item = data - size_of_item;
                for(size_t i=rows;i>0;--i,item+=stride)
                {
                    build_row_at(temp++,item,cols);
                }
            }

        }
    }
    
    bool matrix_metrics:: have_same_sizes(const matrix_metrics &lhs, const matrix_metrics &rhs) throw()
    {
        return (lhs.rows==rhs.rows) && (lhs.cols==rhs.cols);
    }

    bool matrix_metrics:: are_transposed(const matrix_metrics &lhs, const matrix_metrics &rhs) throw()
    {
        return (lhs.rows==rhs.cols) && (lhs.cols==rhs.rows);
    }

}
