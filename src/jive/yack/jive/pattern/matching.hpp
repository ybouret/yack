//! \file

#ifndef YACK_PATTERN_MATCHING_INCLUDED
#define YACK_PATTERN_MATCHING_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        class dictionary;

        //______________________________________________________________________
        //
        //
        //! use a single pattern to match some input
        //
        //______________________________________________________________________
        class matching : public token
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const first_bytes> fbdb; //!< First Bytes DataBase

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            matching(pattern *);                                 //!< setup from new pattern
            matching(const string &, const dictionary * = NULL); //!< compile regexp
            matching(const char   *, const dictionary * = NULL); //!< compile regexp
            matching(const matching &) throw(); //!< copy, without token content
            virtual ~matching()        throw(); //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! search for full match
            /**
             * accepted part is kept in token, even if source is not done
             */
            bool exactly_(source &);

            //! search for partial match
            bool somehow_(source &);

            //! helper for string/text
            template <typename DATANAME> inline
            bool somehow(const DATANAME &data)
            {
                source src( module::open_data(data) );
                return somehow_(src);
            }


            //! helper for string/text
            template <typename DATANAME> inline
            bool extacly(const DATANAME &data)
            {
                source src( module::open_data(data) );
                return exactly_(src);
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const motif scheme; //!< shared pattern
            const bool  strong; //!< if strong
            const bool  feeble; //!< if feeble = !strong
            const fbdb  firsts; //!< firsts allowed bytes
            
        private:
            YACK_DISABLE_ASSIGN(matching);
        };

    }
}

#endif
