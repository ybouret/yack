//! \file


#ifndef YACK_GFX_IMAGE_CODEC_INCLUDED
#define YACK_GFX_IMAGE_CODEC_INCLUDED 1

#include "yack/gfx/image/format/options.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            //__________________________________________________________________
            //
            //
            //! I/O interface for images
            //
            //__________________________________________________________________
            class codec : public article
            {
            public:
                //______________________________________________________________
                //
                // load/read interface
                //______________________________________________________________
                virtual pixmap<rgba> load(const string &filename, const options *) const = 0; //!< load with options
                pixmap<rgba>         load(const char   *filename, const options *) const;     //!< load wrapper



                //! read with textual options
                template <typename FILENAME, typename OPTVALUE> inline
                pixmap<rgba>         read(const FILENAME &filename,
                                          const OPTVALUE &optvalue) const
                {
                    options opts; opts << optvalue;
                    return load(filename,&opts);
                }

                //______________________________________________________________
                //
                // save/emit interface
                //______________________________________________________________
                virtual void save(const pixmap<rgba> &, const string &, const options *) const = 0; //!< save with options
                virtual void save(const pixmap<rgba> &, const char   *, const options *) const;     //!< save wrapper

                //! emit converted picture
                template <typename T, typename FILENAME, typename OPTVALUE, typename PROC> inline
                void emit(const pixmap<T> &source,
                          const FILENAME  &filename,
                          const OPTVALUE  &optvalue,
                          broker          &device,
                          PROC            &toRGBA,
                          pixmap<rgba>    &target) const
                {
                    options opts; opts << optvalue;
                    broker_transform::apply(target,source,device,toRGBA);
                    save(target,filename,&opts);
                }



                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const string &       key() const noexcept; //!< name for ark_ptr

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string name; //!< identifier

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~codec() noexcept;     //!< cleanup

            protected:
                explicit codec(const char *); //!< setup name

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(codec);
            };
        }
    }

}

#endif
