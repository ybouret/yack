//! \file

#ifndef YACK_VIZIBLE_INCLUDED
#define YACK_VIZIBLE_INCLUDED 1

#include "yack/ios/fwd.hpp"

namespace yack
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! GraphViz API
        //
        //______________________________________________________________________
        class vizible
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            ostream & tag(ostream &os)                   const; //!< output address as uuid
            ostream & link(ostream &os, const vizible *) const; //!< prepare arrow


            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static ostream& uuid(ostream &, const void *);                //!< address to hexa
            static ostream& text(ostream &, const char c);                //!< encoded char
            static ostream& text(ostream &, const char *);                //!< encoded chats
            static ostream& end(ostream &);                               //!< append ";\n"
            static ostream& arrow(ostream &, const void *, const void *); //!< prepare arrow
            static ostream& digraph_init(ostream &os, const char *);      //!< init digraph
            static void     digraph_quit(ostream &os);                    //!< quit digraph

            static void render(const char *filename);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~vizible() throw();
        protected:
            explicit vizible() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(vizible);
        };

    }

}

#endif
