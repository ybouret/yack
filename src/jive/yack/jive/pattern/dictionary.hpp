
//! \file

#ifndef YACK_JIVE_DICT_INCLUDED
#define YACK_JIVE_DICT_INCLUDED 1

#include "yack/jive/pattern/regexp.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! base type for dictionary
        //
        //______________________________________________________________________
        typedef suffix_map<string,motif> dictionary_;

        //______________________________________________________________________
        //
        //
        //! API for dictionary
        //
        //______________________________________________________________________
        class dictionary : public dictionary_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit dictionary() noexcept; //!< setup
            virtual ~dictionary() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // query
            //__________________________________________________________________
            const pattern *query(const string &key) const noexcept; //!< query
            const pattern *query(const char   *key) const;         //!< query

            //__________________________________________________________________
            //
            // populate
            //__________________________________________________________________
            void operator()(const string &key, pattern *p); //!< register
            void operator()(const char   *key, pattern *p); //!< register

            //! register from regular expression
            template <typename KEY, typename EXPR>
            inline void decl(const KEY &key, const EXPR &expr)
            {
                dictionary &self = *this;
                self(key, regexp::compile(expr,this) );
            }
            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dictionary);

        };

    }

}

#endif

