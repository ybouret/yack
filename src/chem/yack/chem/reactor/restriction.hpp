
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


            double compute(const readable<double> &source,
                           raddops                &xadd) const throw();

            //! apply restriction
            /**
             target <- source
             */
            double apply(writable<double>       &target,
                         const readable<double> &source,
                         raddops                &xadd) const throw();



            bool overlaps(const restriction &other) const throw();

            size_t operator*() const throw();

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
            explicit rs_group() throw() : object(), rs_list(), next(0), prev(0) {}
            virtual ~rs_group() throw() {}

            // check if the restricition overlaps with one group
            bool overlaps(const restriction &other) const throw()
            {
                for(const rs_node *lhs=head;lhs;lhs=lhs->next)
                {
                    const restriction &mine = **lhs; assert( &mine != &other );
                    if(mine.overlaps(other))
                    {
                        return true;
                    }
                }
                return false;
            }

            // check if an other group overlaps with this
            bool overlaps(const rs_group &other) const throw()
            {
                assert( &other != this );
                for(const rs_node *rhs=other.head;rhs;rhs=rhs->next)
                {
                    if( overlaps(**rhs) ) return true;
                }
                return false;
            }


            rs_group *next;
            rs_group *prev;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rs_group);
        };

        typedef cxx_list_of<rs_group> rs_groups;


    }
}

#endif
