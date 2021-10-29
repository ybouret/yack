#include "yack/utest/driver.hpp"
#include <cstdio>

YACK_UTEST_DECL(80)
{
    YACK_UTEST(crc32);
    YACK_UTEST(error);
    YACK_UTEST(cstring);
    YACK_UTEST(excp);
    YACK_UTEST(base2);
    YACK_UTEST(align);
    YACK_UTEST(at_exit);
    YACK_UTEST(wtime);
    YACK_UTEST(complex);
    YACK_UTEST(gcd64);

    YACK_UTEST(type_ints);
    YACK_UTEST(type_traits);
    YACK_UTEST(type_conv);    
    YACK_UTEST(type_temp);
    YACK_UTEST(type_scalar);
    YACK_UTEST(type_human);
    YACK_UTEST(type_fourcc);
    YACK_UTEST(type_vtx);

    YACK_UTEST(ram);
    YACK_UTEST(memory_chunk);
    YACK_UTEST(memory_arena);
    YACK_UTEST(memory_arena_io);
    YACK_UTEST(memory_alloc);
    YACK_UTEST(memory_book);
    YACK_UTEST(memory_blocks);
    YACK_UTEST(memory_note);
    YACK_UTEST(memory_dyadic);
    YACK_UTEST(memory_objects);
    YACK_UTEST(object);
    YACK_UTEST(memory_parcel);
    YACK_UTEST(memory_parcels);
    YACK_UTEST(memory_buffers);
    YACK_UTEST(memory_embed);
    YACK_UTEST(memory_zcache);

    YACK_UTEST(data_list);
    YACK_UTEST(data_list_sort);
    YACK_UTEST(data_pool);
    YACK_UTEST(data_pool_sort);
    YACK_UTEST(data_small);
    YACK_UTEST(data_matrix);
    YACK_UTEST(data_sequence);


    YACK_UTEST(sync_lockable);
    YACK_UTEST(sync_primitive);
    YACK_UTEST(sync_mutex);
    YACK_UTEST(sync_condition);
    YACK_UTEST(sync_thread);
    YACK_UTEST(singleton);
    YACK_UTEST(sync_loop);
    YACK_UTEST(sync_split);


    YACK_UTEST(fft1d);
    YACK_UTEST(fft_xbr8);
    YACK_UTEST(fft_xbr64);
    YACK_UTEST(fft_xbr_info);

    YACK_UTEST(apex_mem);
    YACK_UTEST(apn);
    YACK_UTEST(apex_ari);
    YACK_UTEST(apex_io);
    YACK_UTEST(apex_perf);

    YACK_UTEST(ios_c_file);
    YACK_UTEST(ios_streams);
    YACK_UTEST(ios_gzstreams);

    YACK_UTEST(hash64);

    YACK_UTEST(rand_bits);

    YACK_UTEST(sort_heap);
    YACK_UTEST(sort_indexing);

    YACK_UTEST(counting_comb);
    YACK_UTEST(counting_perm);

    YACK_UTEST(rtld_dso);

}
YACK_UTEST_EXEC()
