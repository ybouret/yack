//! \file

#ifndef YACK_RTTI_INCLUDED
#define YACK_RTTI_INCLUDED 1

#include "yack/string/fwd.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"
#include <typeinfo>

namespace yack
{


    namespace kernel
    {
        class rtti : public object
        {
        public:
            typedef cxx_pool_of<rtti> pool;
            explicit rtti(const yack::string &);
            virtual ~rtti() throw();

            const yack::string & operator*() const throw();

            rtti         *next;
        private:
            yack::string *impl;
        };
    }

    //__________________________________________________________________________
    //
    //
    //! Run Time Type Information system
    //
    //__________________________________________________________________________
    class rtti : public object, public counted, public kernel::rtti::pool
    {
    public:
        virtual ~rtti() throw();               //!<cleanup
        explicit rtti(const std::type_info &); //!< build with default name

        const string &name() const throw();    //!< get current name

        static const rtti & use(const std::type_info &);
        template <typename T> static inline
        const rtti & use() { return use( typeid(T) ); }

        static const rtti & use(const std::type_info &, const string &);
        static const rtti & use(const std::type_info &, const char   *);

        template <typename T, typename ALIAS> static inline
        const rtti & use(const ALIAS &alias) { return use( typeid(T), alias); }

        friend std::ostream & operator<<(std::ostream &, const rtti &);

        static void gv();
        static void display();
        
        static const string &name(const std::type_info &);
        template <typename T> static inline
        const string & name() { return name( typeid(T) ); }
        



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(rtti);
    };

}

#endif

