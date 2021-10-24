#include "yack/container/matrix/metrics.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/global.hpp"

namespace yack
{
    
    matrix_metrics:: ~matrix_metrics() throw()
    {
        deallocate();
    }
    
    void matrix_metrics:: deallocate() throw()
    {
        if(rows)
        {
            static memory::allocator   &mem = memory::global::location();
            mem.release(workspace,coerce(allocated));
            coerce(items) = 0;
            coerce(cols)  = 0;
            coerce(rows)  = 0;

        }
    }
    
    
    matrix_metrics:: matrix_metrics( const size_t size_of_item) throw():
    rows(0),
    cols(0),
    items(0),
    item_size(size_of_item),
    allocated(0),
    workspace(0)
    {
    }
    
    
    matrix_metrics:: matrix_metrics(void **       row_hook,
                                    const size_t  num_rows,
                                    const size_t  num_cols,
                                    const size_t  size_of_item) :
    rows(num_rows),
    cols(num_cols),
    items(rows*cols),
    item_size(size_of_item),
    allocated(0),
    workspace(0)
    {
        // sanity check
        
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
            static memory::allocator   &mem = memory::global::instance();
            typedef matrix_row<uint8_t> row_t;
            const size_t stride = cols*item_size;
            
            // allocate
            row_t               *temp = 0;
            uint8_t             *data = 0;
            memory::embed        emb[] =
            {
                memory::embed(temp,rows),
                memory::embed(data,items*item_size)
            };
            workspace = YACK_MEMORY_EMBED(emb,mem,coerce(allocated));
            
            // link
            *row_hook =  out_of_reach::address(temp-1);
            for(size_t i=1;i<=rows;++i,data+=stride)
            {
                new (temp++) row_t(data,cols);
            }
            
        }
    }
    
    
}
