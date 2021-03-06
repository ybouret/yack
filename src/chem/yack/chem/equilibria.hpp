//! \file

#ifndef YACK_CHEM_EQUILIBRIA_INCLUDED
#define YACK_CHEM_EQUILIBRIA_INCLUDED 1

#include "yack/chem/eqs/const.hpp"
#include "yack/chem/compendium.hpp"


namespace yack
{
    namespace chemical
    {
        class library;

        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef suffix_set<string,const equilibrium::pointer> eq_set; //!< alias
        typedef eq_set::knot_type                             enode;  //!< alias

        //______________________________________________________________________
        //
        //
        //!set of unique equilibrium
        //
        //______________________________________________________________________
        class equilibria : public compendium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria() throw(); //!< setup empty
            virtual ~equilibria() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual size_t size() const throw(); //!< size
            const enode   *head() const throw(); //!< first equilibrium

            //! try to insert a new equilibrium
            equilibrium & use( equilibrium *eq );


            //! try to insert a precomputed equilibrium
            void add( const equilibrium::pointer &pEq );

            //! try to declare a new constant equilibrium
            template <typename ID> inline
            equilibrium & create(const ID &name, const double K)
            {
                return use( new const_equilibrium(name,db.size()+1,K) );
            }

            //! formatted display
            friend std::ostream & operator<<(std::ostream &, const equilibria &);


            //! display named values
            template <typename ARR> inline
            void operator()(std::ostream &os,
                            ARR          &arr,
                            const char   *pfx = NULL) const
            {
                if(!pfx) pfx = "";
                os << '{';
                for(const enode *node=head();node;node=node->next)
                {
                    const equilibrium &s = ***node;
                    os << std::endl;
                    pad(os << pfx << ' ' << '{' << s.name << '}',s) << " = " << arr[*s];
                }
                os << ' ' << '}' << std::endl;
            }

            //! fetch (slow helper)
            const equilibrium & operator[](const size_t indx) const throw();

            //! duplicate
            equilibria & operator<<(const equilibria &other);

            //! build detached matrix
            void build(matrix<bool> &detached) const;

            //! build topology matrix and initial constants
            template <typename T> inline
            void build( matrix<T> &Nu, writable<double> &K, const double t) const
            {
                assert(size()==Nu.rows);
                for(const enode *node=head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    eq.must_comply(eq.name);
                    eq.fill(Nu[ei]);
                    K[ei] = eq.K(t);
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
            eq_set db;
        };

    }

}

#endif

