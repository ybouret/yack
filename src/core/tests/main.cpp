#include "yack/utest/driver.hpp"
#include <cstdio>

YACK_UTEST_DECL(256)
{
    YACK_UTEST(native);
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
    YACK_UTEST(prime);
    YACK_UTEST(primePi);
    YACK_UTEST(prime32);
    YACK_UTEST(prime_stab);
    YACK_UTEST(genp32);
    YACK_UTEST(functor);
    YACK_UTEST(rtti);
    YACK_UTEST(env);
    YACK_UTEST(div);
    YACK_UTEST(zflux);
    YACK_UTEST(net128);
    YACK_UTEST(abs);
    YACK_UTEST(round10);
    YACK_UTEST(integral);

    YACK_UTEST(type_ints);
    YACK_UTEST(type_traits);
    YACK_UTEST(type_conv);    
    YACK_UTEST(type_temp);
    YACK_UTEST(type_scalar);
    YACK_UTEST(type_human);
    YACK_UTEST(type_fourcc);
    YACK_UTEST(type_vtx);
    YACK_UTEST(type_constant);
    YACK_UTEST(type_gateway);
    YACK_UTEST(type_dec8);

    YACK_UTEST(memory_ram);
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
    YACK_UTEST(large_object);
    YACK_UTEST(spot_object);
    YACK_UTEST(memory_parcel);
    YACK_UTEST(memory_parcels);
    YACK_UTEST(memory_buffers);
    YACK_UTEST(memory_buff_io);
    YACK_UTEST(memory_embed);
    YACK_UTEST(memory_zcache);
    YACK_UTEST(memory_operative);
    YACK_UTEST(memory_ptr);
    YACK_UTEST(memory_sentry);
    YACK_UTEST(memory_contractor);
    YACK_UTEST(memory_shelf);

    YACK_UTEST(data_list);
    YACK_UTEST(data_list_sort);
    YACK_UTEST(data_list_ordered);
    YACK_UTEST(data_pool);
    YACK_UTEST(data_pool_sort);
    YACK_UTEST(data_small);
    YACK_UTEST(data_matrix);
    YACK_UTEST(data_sequence);
    YACK_UTEST(data_cxx_series);
    YACK_UTEST(data_pqueue);
    YACK_UTEST(data_heap);
    YACK_UTEST(data_tree);
    YACK_UTEST(data_hash);
    YACK_UTEST(data_iter);
    YACK_UTEST(data_key_variety);
    YACK_UTEST(data_hash_map);
    YACK_UTEST(data_hash_set);
    YACK_UTEST(data_suffix_map);
    YACK_UTEST(data_suffix_set);
    YACK_UTEST(data_registry);
    YACK_UTEST(data_glossary);
    YACK_UTEST(data_arrays);
    YACK_UTEST(data_factory);
    YACK_UTEST(data_stats);
    YACK_UTEST(data_pipes);
    YACK_UTEST(data_addrbook);
    YACK_UTEST(data_lexicon);
    YACK_UTEST(data_key_mapper);
    YACK_UTEST(data_doubly);
    YACK_UTEST(data_groove);
    YACK_UTEST(data_sorted_list);
    YACK_UTEST(data_be_key);
    YACK_UTEST(data_pair);
    YACK_UTEST(data_triple);
    YACK_UTEST(data_quad);
    YACK_UTEST(data_roll);
    YACK_UTEST(data_bunch);
    YACK_UTEST(data_set);
    YACK_UTEST(data_roster);
    YACK_UTEST(data_clist);

    YACK_UTEST(dinky_znone);
    YACK_UTEST(dinky_zsolo);
    YACK_UTEST(dinky_zcoop);
    YACK_UTEST(dinky_zproxy);
    YACK_UTEST(dinky_root);
    YACK_UTEST(dinky_registry);
    YACK_UTEST(dinky_manifest);
    YACK_UTEST(dinky_ops);
    YACK_UTEST(dinky_core_list);
    YACK_UTEST(dinky_core_repo);
    YACK_UTEST(dinky_solo_list);
    YACK_UTEST(dinky_solo_repo);
    YACK_UTEST(dinky_coop_list);
    YACK_UTEST(dinky_coop_repo);


    YACK_UTEST(arith_isqrt);

    YACK_UTEST(sync_lockable);
    YACK_UTEST(sync_primitive);
    YACK_UTEST(sync_mutex);
    YACK_UTEST(sync_condition);
    YACK_UTEST(sync_thread);
    YACK_UTEST(singleton);
    YACK_UTEST(sync_loop);
    YACK_UTEST(sync_split);
    YACK_UTEST(sync_queue);
    YACK_UTEST(sync_topology);


    YACK_UTEST(fft1d);
    YACK_UTEST(fft_xbr8);
    YACK_UTEST(fft_xbr64);
    YACK_UTEST(fft_xbr_info);

    YACK_UTEST(apex_mem);
    YACK_UTEST(apex_info);
    YACK_UTEST(apn);
    YACK_UTEST(apz);
    YACK_UTEST(apq);
    YACK_UTEST(apex_ari);
    YACK_UTEST(apex_io);
    YACK_UTEST(apex_perf);
    YACK_UTEST(apex_conv);
    YACK_UTEST(apfac);
    YACK_UTEST(aprsa);
    YACK_UTEST(apr);
    YACK_UTEST(aprimes);
    YACK_UTEST(aprimes64);
    YACK_UTEST(apex_npd);
    YACK_UTEST(alga);
    YACK_UTEST(apex_isqrt);
    YACK_UTEST(apex_flak);

    YACK_UTEST(ios_c_file);
    YACK_UTEST(ios_streams);
    YACK_UTEST(ios_vstream);
    YACK_UTEST(ios_gz);
    YACK_UTEST(ios_bz);
    YACK_UTEST(ios_fmt);
    YACK_UTEST(ios_gets);
    YACK_UTEST(ios_ascii);
    YACK_UTEST(ios_hybrid);
    YACK_UTEST(ios_convert);
    YACK_UTEST(ios_loader);
    YACK_UTEST(ios_scribe);
    YACK_UTEST(ios_markup);
    YACK_UTEST(ios_pstream);
    YACK_UTEST(ios_bits);
    YACK_UTEST(ios_xmlog);
    YACK_UTEST(ios_entity);

    YACK_UTEST(hash64);
    YACK_UTEST(hashing_fcn);
    YACK_UTEST(hashing_hmac);
    YACK_UTEST(hashing_key);
    YACK_UTEST(hashing_perfect);

    YACK_UTEST(rand_bits);
    YACK_UTEST(rand_metrics);
    YACK_UTEST(rand_shuffle);
    YACK_UTEST(rand_mt19937);
    YACK_UTEST(rand_gaussian);

    YACK_UTEST(sort_heap);
    YACK_UTEST(sort_indexing);
    YACK_UTEST(sort_nw);
    YACK_UTEST(sort_network);
    YACK_UTEST(sort_nws);
    YACK_UTEST(sort_quick);

    YACK_UTEST(counting_comb);
    YACK_UTEST(counting_perm);
    YACK_UTEST(counting_part);
    YACK_UTEST(counting_loop);
    YACK_UTEST(counting_limits);
    YACK_UTEST(counting_card);
    YACK_UTEST(counting_binary);

    YACK_UTEST(rtld_dso);
    
    YACK_UTEST(string_utf8);
    YACK_UTEST(strings);
    YACK_UTEST(string_tokenize);

    YACK_UTEST(info_bwt);
    YACK_UTEST(info_mod);
    YACK_UTEST(info_xbwt);
    YACK_UTEST(info_alphabet);

    YACK_UTEST(math_api);
    YACK_UTEST(math_extended);
    YACK_UTEST(math_xcomplex);

    YACK_UTEST(vfs);
    YACK_UTEST(vfs_local);
    YACK_UTEST(vfs_attr);
    YACK_UTEST(vfs_mkdir);
    YACK_UTEST(vfs_bytes);

    YACK_UTEST(kr_digest);
    YACK_UTEST(kr_gray);
    YACK_UTEST(kr_tea);
    YACK_UTEST(kr_aes);
    YACK_UTEST(kr_bc);
    YACK_UTEST(kr_op);

    YACK_UTEST(fields);

    YACK_UTEST(color_rgb);
    YACK_UTEST(color_yuv);
    YACK_UTEST(color_unit_real);
    YACK_UTEST(color_gs);

    YACK_UTEST(raven_space);
    YACK_UTEST(raven_family);

    YACK_UTEST(cameo_add);
    YACK_UTEST(cameo_static_add);
    YACK_UTEST(cameo_mul);

    YACK_UTEST(ipso_mul);
    YACK_UTEST(ipso_add);
    YACK_UTEST(ipso_add_static);

    YACK_UTEST(woven);
}
YACK_UTEST_EXEC()
