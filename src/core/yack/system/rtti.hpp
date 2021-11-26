//! \file

#ifndef YACK_RTTI_INCLUDED
#define YACK_RTTI_INCLUDED 1

#include "yack/string/fwd.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"
#include <typeinfo>
#include <iosfwd>

namespace yack
{


    namespace kernel
    {
        //______________________________________________________________________
        //
        //! run time type information alias
        //______________________________________________________________________
        class rtti : public object
        {
        public:
            typedef cxx_pool_of<rtti> pool;                       //!< to store aliases
            explicit                  rtti(const yack::string &); //!< create string
            virtual                  ~rtti()            throw();  //!< cleanup
            const yack::string &      operator*() const throw();  //!< access alias
            rtti                     *next;                       //!< for pool
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rtti);
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
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~rtti() throw();               //!<cleanup
        explicit rtti(const std::type_info &); //!< build with default name

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const string &name()   const throw();    //!< get current name
        const string &native() const throw();    //!< get native name

        static                              const rtti & use(const std::type_info &);       //!< declare/recall typeid
        template <typename T> static inline const rtti & use() { return use( typeid(T) ); } //!< declare/recall typeid


        static const rtti & use(const std::type_info &, const string &);        //!< declare/recall typeid, check/add alias
        static const rtti & use(const std::type_info &, const char   *);        //!< declare/recall typeid, check/add alias
        template <typename T, typename ALIAS> static inline                     //|
        const rtti & use(const ALIAS &alias) { return use( typeid(T), alias); } //!< declare/recall typeid, check/add/alias


        friend std::ostream & operator<<(std::ostream &, const rtti &);//!< output all aliases


        static                              const string & name(const std::type_info &);        //!< get name
        template <typename T> static inline const string & name() { return name( typeid(T) ); } //!< get name
        
        static void gv();        //!< make a graph
        static void display();   //!< make a graph


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(rtti);
    };

}

#endif

