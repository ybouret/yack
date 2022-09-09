//! \file

#ifndef YACK_INFO_COMPRESSION_ALPHABET_INCLUDED
#define YACK_INFO_COMPRESSION_ALPHABET_INCLUDED 1

#include "yack/data/list/raw.hpp"
#include "yack/type/ints.hpp"
#include "yack/memory/embed.hpp"
#include <iostream>

namespace yack
{
    namespace information
    {

        namespace codec
        {
            //__________________________________________________________________
            //
            //
            //! escape sequence naming
            //
            //__________________________________________________________________
            enum escape_glyph
            {
                escape_end=0, //!< flushing
                escape_nyt=1, //!< Not Yet Transmitted
            };

            typedef int2type<escape_end> ESC_END_t; //!< alias
            extern const ESC_END_t       ESC_END;   //!< alias
            
            typedef int2type<escape_nyt> ESC_NYT_t; //!< alias
            extern const ESC_NYT_t       ESC_NYT;   //!< alias



            //__________________________________________________________________
            //
            //
            //! anonymous glyph operations
            //
            //__________________________________________________________________
            struct glyph_ops
            {
                //! display raw data
                static void display_raw(std::ostream &os, const void *addr, const size_t size);

                static memory::allocator & memmgr(); //!< memory manager
                static void                release(void * &, size_t &) throw(); //!< release allocated bytes
            };

            //__________________________________________________________________
            //
            //
            //! generic glyph
            //
            //__________________________________________________________________
            template <const size_t CODE_BITS>
            class glyph
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static  const    size_t    code_bits = CODE_BITS;         //!< code bits
                static  const    size_t    code_size = CODE_BITS/8;       //!< code bytes
                static  const    size_t    word_size = code_size*2;       //!< word bytes, greater than code bytes
                static  const    size_t    word_bits = word_size*8;       //!< word bits
                typedef typename unsigned_int<code_size>::type code_type; //!< matching code type
                typedef typename unsigned_int<word_size>::type word_type; //!< matching word type
                static  const    word_type cntl_mini = (1<<code_bits);    //!< minimum control value
                static  const    size_t    cntl_bits = code_bits+1;       //!< bits for control, one more than code
                static  const    word_type cntl_maxi = (1<<cntl_bits)-1;  //!< maximum control value

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline  glyph(const code_type c)          throw() : code(c), bits(code_bits), freq(0), next(0), prev(0) {}   //!< setup
                inline ~glyph()                           throw() { }                               //!< cleanup


                //! assign an escape/control sequence
                template <int ESC>
                inline glyph(const int2type<ESC> &_) throw() : code(cntl_mini+_.value), bits(cntl_bits), freq(1), next(0), prev(0)
                {
                }


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! display (raw)
                inline friend std::ostream & operator<<(std::ostream &os, const glyph &g)
                {
                    if(g.bits<=code_bits)
                    {
                        glyph_ops::display_raw(os, & g.code, code_size );
                    }
                    else
                    {
                        os << "[ESC+" << int(g.code-cntl_mini) << "]";
                    }
                    return os;
                }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const word_type code; //!< code on a word
                const size_t    bits; //!< bits
                const size_t    freq; //!< frequency
                glyph          *next; //!< for list
                glyph          *prev; //!< for list

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(glyph);
            };


            //! alpabet status
            enum alphabet_status
            {
                alphabet_set, //!< empty
                alphabet_run, //!< has some codes, not all
                alphabet_all  //!< has all codes
            };


            //! alphabet
            template <const size_t CODE_BITS>
            class alphabet
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef glyph<CODE_BITS>               glyph_type;                                    //!< alias
                typedef raw_list_of<glyph_type>        glyph_list;                                    //!< alias
                typedef typename glyph_type::code_type code_type;                                     //!< alias
                typedef typename glyph_type::word_type word_type;                                     //!< alias
                static  const size_t                   code_bits  = glyph_type::code_bits;            //!< alias
                static  const size_t                   num_codes  = (1<<code_bits);                   //!< codes
                static  const size_t                   num_cntls  = escape_end+1;                     //!< controls
                static  const size_t                   num_glyphs = num_codes+num_cntls;              //!<  glyphs
                static  const size_t                   data_size  = num_glyphs * sizeof(glyph_type);  //!< bytes to hold glyphs

                //! setup with data_size workspace
                inline explicit alphabet(void *wksp) throw() :
                g_list(),
                status(alphabet_set),
                tab( static_cast<glyph_type *>(wksp) ),
                end( tab+num_codes ),
                nyt( end+1 )
                {
                    setup();
                }

                //! cleanup
                inline virtual ~alphabet() throw()
                {
                    g_list.restart();
                    out_of_reach::zset(tab,data_size);
                }

                //! access
                inline const glyph_list * operator->() const throw() { return &g_list; }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(alphabet);
                glyph_list            g_list;
            public:
                const alphabet_status status; //!< current status
            private:
                glyph_type *tab;
                glyph_type *end;
                glyph_type *nyt;


                void setup() throw()
                {
                    for(size_t i=0;i<num_codes;++i)
                        new (tab+i) glyph_type(i);
                    g_list.push_back( new (end) glyph_type(ESC_END) );
                    g_list.push_back( new (nyt) glyph_type(ESC_NYT) );
                }
            };
            

        }



    }
}

#endif