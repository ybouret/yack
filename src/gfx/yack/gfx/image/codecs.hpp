//! \file


#ifndef YACK_GFX_IMAGE_CODECS_INCLUDED
#define YACK_GFX_IMAGE_CODECS_INCLUDED 1

#include "yack/gfx/image/codec.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            //__________________________________________________________________
            //
            //
            // aliases
            //
            //__________________________________________________________________
            typedef ark_ptr<string,const codec>  codec_ptr; //!< shared codec
            typedef suffix_set<string,codec_ptr> codecs_db; //!< database

            //__________________________________________________________________
            //
            //
            //! shared database of codecs
            //
            //__________________________________________________________________
            class codecs : public singleton<codecs>, public codec, public codecs_db
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const char               call_sign[];       //!< for singleton
                static const at_exit::longevity life_time = 1001;  //!< for singleton

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                virtual pixmap<rgba> load(const string &, const options *) const; //!< load by extension


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void         decl(codec *);                       //!< register a new codec
                const codec *query(const string &) const throw(); //!< query existing codec
                const codec *query(const char   *) const throw(); //!< query existing codec

                //! on-the-fly get, assuming CODEC::name() == CODEC::clid
                template <typename CODEC> inline
                CODEC & get()
                {
                    const codec *ptr = query(CODEC::clid);
                    if(ptr)
                    {
                        return coerce(*dynamic_cast<const CODEC *>(ptr));
                    }
                    else
                    {
                        CODEC *ptr = new CODEC();
                        decl(ptr);
                        return *ptr;
                    }
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(codecs);
                explicit codecs();
                virtual ~codecs() throw();
                friend class singleton<codecs>;
            };
        }

        typedef image::codecs images; //!< alias
    }

}

#endif

