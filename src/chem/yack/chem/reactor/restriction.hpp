
#ifndef YACK_CHEMICAL_RESTRICTION_INCLUDED
#define YACK_CHEMICAL_RESTRICTION_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    class xmlog;

    namespace chemical
    {

        //! restriction on positive concentration
        class restriction : public object, public counted, public actors
        {
        public:
            explicit restriction() throw();
            virtual ~restriction() throw();

            bool compile(const size_t idx) throw();

            //! apply restriction
            /**
             target <- source
             */
            double apply(writable<double>       &target,
                         const readable<double> &source,
                         raddops                &xadd) const;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(restriction);
            const unsigned q2; //!< sum of squared coefficients
            const size_t   ii; //!< index
        };

        
        typedef meta_list<const restriction> rs_list;
        typedef rs_list::node_type           rs_node;

        class rs_group : public object, public rs_list
        {
        public:
            explicit rs_group() throw();
            virtual ~rs_group() throw();

            rs_group *next;
            rs_group *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rs_group);
        };

        typedef cxx_list_of<rs_group> rs_groups;

    }
}

#endif
