//! \file

#ifndef YACK_GFX_IMAGE_CODECS_INCLUDED
#define YACK_GFX_IMAGE_CODECS_INCLUDED 1

#include "yack/gfx/image/format.hpp"
#include "yack/gfx/filters.hpp"

namespace yack
{
    namespace graphic
    {

        typedef ark_ptr<string,const filter>   filter_ptr;
        typedef suffix_set<string,filter_ptr>  filter_set;

        //__________________________________________________________________
        //
        //
        //! light-weight shared filters
        //
        //__________________________________________________________________
        class filters
        {
        public:
            static const char Xext[];
            static const char Yext[];

            filters(const filter &fx, const filter &fy) throw();
            filters(const filters &)                    throw();
            virtual ~filters() throw();

            const filter_ptr X;
            const filter_ptr Y;
        private:
            YACK_DISABLE_ASSIGN(filters);
        };

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
                virtual pixmap<rgba> load(const string &, const options *)                       const; //!< load by extension
                virtual void         save(const pixmap<rgba> &, const string &, const options *) const; //!< save by extension

                //______________________________________________________________
                //
                // methods for formats
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
                formats &decl_standard();

                //! find format for filename, based on extension
                const  format  & format_for(const string &filename) const;

                //! get instance + decl_built_in
                static formats & standard();

                //______________________________________________________________
                //
                // methods for filters
                //______________________________________________________________
                //! declare a new filter
                void decl(filter *);

                //! declare two filters based on tab
                template <typename T> inline
                void create_filters(const string &root,
                                    const T     *tab,
                                    const unit_t nx,
                                    const unit_t ny)
                {

                    {
                        const string     id = root+filters::Xext;
                        decl( new filter(id,tab,nx,ny,filter::direct) );
                    }
                    
                    {
                        const string     id = root+filters::Yext;
                        decl( new filter(id,tab,nx,ny,filter::rotate) );
                    }
                }



                template <typename FILTER> inline
                void create()
                {
                    const string root = FILTER::uuid;
                    create_filters(root, & FILTER::data[0][0], FILTER::size, FILTER::size);
                }

                const filter     &fetch(const string &id)   const;
                const filter     &fetch(const char   *id)   const;
                filters           carve(const string &root) const;
                filters           carve(const char   *root) const;

                const filter_set fdb;

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

