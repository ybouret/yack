//! \file

#ifndef YACK_ORDERED_ROSTER_INCLUDED
#define YACK_ORDERED_ROSTER_INCLUDED 1

#include "yack/data/dinky/solo-list.hpp"
#include "yack/container.hpp"

namespace yack
{

    namespace low_level
    {
        extern const char roster_category[]; //!< unique ID
    }

    //__________________________________________________________________________
    //
    //
    //! ordered list with sign_type COMPARATOR(lhs,rhs)
    //
    //__________________________________________________________________________
    template <typename T, typename COMPARATOR>
    class roster : public container
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                          //!< aliases
        typedef solo_list<T>                  list_type; //!< alias
        typedef typename list_type::node_type node_type; //!< alias
        typedef list_of<node_type>            base_list; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit roster() noexcept : container(), my(), cm() {} //!< setup empty
        inline virtual ~roster() noexcept {}                           //!< cleanup

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        inline virtual const char *category()  const noexcept { return low_level::roster_category; }
        inline virtual size_t      size()      const noexcept { return my.size; }
        inline virtual size_t      capacity()  const noexcept { return my.size+my.ready(); }
        inline virtual size_t      available() const noexcept { return my.ready();}
        inline virtual void        free()            noexcept { my.clear(); }
        inline virtual void        release()         noexcept { my.clear(); my.cache->release(); }
        inline virtual void        reserve(size_t n)          { return my.cache->reserve(n); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! low-level access
        inline const base_list * operator->() const noexcept { return &my; }

        //! insert a new value
        inline void insert(param_type args)
        {
            node_type  *node = my.new_node(args);
            try {
                switch(my.size)
                {
                    case 0:  my.push_back(node);   break; // direct
                    case 1:  insert_second(node);  break; // special case
                    default: insert_higher(node);  break; // generic case
                }
                assert(is_valid());
            }
            catch(...) { my.zombify(node); throw; }
        }

        //! insert a new value a few times
        //! 
        inline void insert(param_type args, unsigned n)
        {
            base_list rep;
            try {
                // create sub-list
                while(n-- > 0) rep.push_back( my.new_node(args) );

                // dispatch
                switch(my.size)
                {
                        // trivial case
                    case 0: my.swap_with(rep); break;

                        // specific case
                    case 1:
                        switch( cm(args,**my.head) )
                        {
                            case negative: my.merge_front(rep); break;
                            case __zero__:
                            case positive: my.merge_back(rep);  break;
                        }
                        break;

                        // generic case
                    default:
                        node_type *prec = find_preceding(args);
                        if(!prec)
                            my.merge_front(rep);
                        else
                            my.insert_after(prec,rep);
                        break;
                }
            }
            catch(...)
            {
                while(rep.size) my.zombify(rep.pop_back());
                throw;
            }
        }

        //! display internal list
        inline friend std::ostream & operator<<(std::ostream &os, const roster &self)
        {
            os << self.my;
            return os;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(roster);
        list_type          my;
        mutable COMPARATOR cm;

        inline bool is_valid() const {
            const node_type *curr = my.head;
            while(curr && curr->next)
            {
                const node_type *next = curr->next;
                switch(cm(**curr,**next))
                {
                    case __zero__:
                    case negative: break;
                    case positive: return false;
                }
                curr = next;
            }
            return true;
        }

        inline void insert_second(node_type *node) noexcept
        {
            assert(1==my.size);
            switch( cm(**node,**my.head) )
            {
                case negative: my.push_front(node); break;
                case __zero__:
                case positive: my.push_back(node);  break;
            }
        }

        inline node_type *find_preceding(const_type &value)
        {
            assert(my.size>=2);

            //------------------------------------------------------------------
            // test against lower bound
            //------------------------------------------------------------------
            node_type  *lower  = my.head;
            switch( cm(value,**lower) )
            {
                case negative: return NULL; // before head
                case __zero__:              // after  head
                case positive: break;       // after  head
            }

            //------------------------------------------------------------------
            // test against upper bound
            //------------------------------------------------------------------
            node_type *upper = my.tail; assert(upper!=lower);
            switch( cm(**upper,value) )
            {
                case __zero__:               // after  upper
                case negative: return upper; // after  upper
                default:              break; // before upper
            }

        PROBE:
            node_type  *probe = lower->next; assert(probe!=NULL);
            if(probe==upper) goto RETURN; // mandatory

            switch( cm(value,**probe) )
            {
                case negative: goto RETURN;
                case __zero__:
                case positive: lower=probe; goto PROBE;
            }

        RETURN:
            return lower;

        }

        inline void insert_higher(node_type *node)
        {
            assert(node); assert(my.size>=2);
            node_type *prec = find_preceding(**node);
            if(!prec)
                (void) my.push_front(node);
            else
                (void) my.insert_after(prec,node);
        }
    };

}

#endif

