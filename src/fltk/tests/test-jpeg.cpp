#include "yack/utest/run.hpp"

#include <errno.h>
#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
YACK_UTEST(jpeg)
{
    const char *filename = "./etc/nhe2.jpg";
    fl_register_images();                       // initialize image lib
    Fl_Window     win(720,486);                 // make a window
    Fl_Box        box(10,10,720-20,486-20);     // widget that will contain image
    Fl_JPEG_Image jpg(filename);                // load jpeg image into ram
                                                // Error checking
    switch ( jpg.fail() ) {
        case Fl_Image::ERR_NO_IMAGE:
        case Fl_Image::ERR_FILE_ACCESS:
            fl_alert("%s: %s", filename, strerror(errno));    // shows actual os error to user
            return 1;
        case Fl_Image::ERR_FORMAT:
            fl_alert("%s: couldn't decode image", filename);
            return 1;
    }
    box.image(jpg);                             // attach jpg image to box
    win.show();
    return(Fl::run());
}
YACK_UDONE()
