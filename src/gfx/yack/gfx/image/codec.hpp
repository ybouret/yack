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
                // interface
                //______________________________________________________________
                virtual pixmap<rgba> load(const string &filename, const options *) const = 0; //!< load with options
                pixmap<rgba>         load(const char   *filename, const options *) const;     //!< load wrapper
                template <typename FILENAME, typename OPTVALUE> inline
                pixmap<rgba>         read(const FILENAME &filename,
                                          const OPTVALUE &optvalue) const
                {
                    options opts; opts << optvalue;
                    return load(filename,&opts);
                }


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const string &       key() const throw(); //!< name for ark_ptr

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string name; //!< identifier

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~codec() throw();     //!< cleanup

            protected:
                explicit codec(const char *); //!< setup name

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(codec);
            };
        }
    }

}

#endif
