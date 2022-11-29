
#ifndef YACK_CHEMICAL_RESTRICTION_INCLUDED
#define YACK_CHEMICAL_RESTRICTION_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    class xmlog;

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! restriction on positive concentration
        //
        //______________________________________________________________________
        class restriction : public object, public counted, public actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit restriction() throw(); //!< setup empty
            virtual ~restriction() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! prepare factor and set index
            bool compile(const size_t idx) throw();

            //! compute current state
            double compute(const readable<double> &source,
                           raddops                &xadd) const throw();

            //! apply restriction
            double apply(writable<double>       &target,
                         const readable<double> &source,
                         raddops                &xadd) const throw();

            //! check if this has a common species with other
            bool overlaps(const restriction &other) const throw();

            //! get index
            size_t operator*() const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(restriction);
            const unsigned q2; //!< sum of squared coefficients
            const size_t   ii; //!< index
        };

        
        typedef meta_list<const restriction> rs_list; //!< alias
        typedef rs_list::node_type           rs_node; //!< alias


        //______________________________________________________________________
        //
        //
        //! group of dependent restrictions
        //
        //______________________________________________________________________
        class rs_group : public object, public rs_list
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit rs_group() throw() : object(), rs_list(), next(0), prev(0) {} //!< setup empty
            virtual ~rs_group() throw() {}                                         //!< cleanu

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! check if the restricition overlaps with one group
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

            //! check if an other group overlaps with this
            bool overlaps(const rs_group &other) const throw()
            {
                assert( &other != this );
                for(const rs_node *rhs=other.head;rhs;rhs=rhs->next)
                {
                    if( overlaps(**rhs) ) return true;
                }
                return false;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            rs_group *next; //!< for groups
            rs_group *prev; //!< for groups

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rs_group);
        };

        typedef cxx_list_of<rs_group> rs_groups; //!< alias


    }
}

#endif
