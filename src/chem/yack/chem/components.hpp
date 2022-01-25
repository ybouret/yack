
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/component.hpp"

namespace yack
{

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! base class for components
        //
        //______________________________________________________________________
        typedef suffix_set<string,component::pointer> components_;
        class   library;


        //______________________________________________________________________
        //
        //
        //! set of components
        //
        //______________________________________________________________________
        class components : public components_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~components() throw(); //!< cleanup
            explicit components() throw(); //!< setup

            //! helper to form and insert a new component
            void create(const species  &sp,
                        const unit_t    nu);

            //! compile expression
            void operator()(const string &expr, library &lib);

            //! compile expression
            void operator()(const char   *expr, library &lib);

            //! display
            void display(std::ostream&) const;

            //! display
            friend std::ostream &operator<<(std::ostream &os, const components &);




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(components);
        };
    }

}

#endif
