//! \file


#ifndef YACK_GFX_IMAGE_CODECS_INCLUDED
#define YACK_GFX_IMAGE_CODECS_INCLUDED 1

#include "yack/gfx/image/format.hpp"
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
            typedef ark_ptr<string,const format>   fmt_ptr; //!< shared codec
            typedef suffix_set<string,fmt_ptr>     fmt_set; //!< database

            //__________________________________________________________________
            //
            //
            //! shared database of codecs
            //
            //__________________________________________________________________
            class formats : public singleton<formats>, public codec, public fmt_set
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
                void          decl(format *);                      //!< register a new format
                const format *query(const string &) const throw(); //!< query existing format
                const format *query(const char   *) const throw(); //!< query existing format

                //! on-the-fly get, assuming CODEC::name() == CODEC::clid
                template <typename CODEC> inline
                CODEC & get()
                {
                    const format *ptr = query(CODEC::clid);
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

                //! load built-in formats
                formats &decl_built_in();

                //! find format for filename, based on extension
                const  format  & format_for(const string &filename) const;

                //! get instance + decl_built_in
                static formats & standard();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(formats);
                explicit formats();
                virtual ~formats() throw();
                friend class singleton<formats>;
            };
        }

        typedef image::formats images; //!< alias
    }

}

#endif

