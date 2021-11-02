//! \file

#ifndef YACK_INFO_BWT_INCLUDED
#define YACK_INFO_BWT_INCLUDED 1


#include "yack/info/modulation.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! forward Burrows Wheeler Transform, returns the primary index */
    size_t yack_bwt_encode(void        *output,
                           const void  *input,
                           const size_t size,
                           size_t      *indices);



    /**! forward Burrows Wheeler Transform with modulation */
    size_t yack_bwt_xencode(void           *output,
                            const void     *input,
                            const size_t    size,
                            size_t         *indices,
                            yack_modulation call,
                            void           *args);

    /**! reverse Burrows Wheeler Transform  using the primary index */
    void   yack_bwt_decode(void        *output,
                           const void  *input,
                           const size_t size,
                           size_t      *indices,
                           const size_t primary_index);

    /**! reverse Burrows Wheeler Transform  using the primary index */
    /**
     warning: input is modified!
     */
    void   yack_bwt_xdecode(void           *output,
                            void           *input,
                            const size_t    size,
                            size_t         *indices,
                            const size_t    primary_index,
                            yack_modulation call,
                            void           *args);


#if defined(__cplusplus)
}
#endif

#endif
