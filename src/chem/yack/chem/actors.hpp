
//! \file

#ifndef YACK_CHEMICAL_ACTORS_INCLUDED
#define YACK_CHEMICAL_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/types.hpp"
#include "yack/chem/xlimit.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/bare.hpp"
#include "yack/container/tuple/pair.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! building a pair of value/index for greatest value detection
        //
        //______________________________________________________________________
        YACK_PAIR_DECL(STANDARD,greatest,const double,value,const size_t,index);

        //! setup empty
        inline greatest() throw() : value(0), index(0) {}

        //! update
        inline greatest & operator<<(const greatest &other) throw() {
            if(other.value>value) {
                coerce(value) = other.value;
                coerce(index) = other.index;
            }
            return *this;
        }


        //! divide by the greatest value, taking care of index sign
        template <typename ARR> inline
        void divide(ARR &arr) const throw() {
            assert(fabs(value)>0);
            assert(index>0);
            const size_t n=arr.size();
            for(size_t i=n;i>index;--i)   arr[i] /= value;
            arr[index] = one(arr[index]);
            for(size_t i=index-1;i>0;--i) arr[i] /= value;
        }
        

    private:
        template <typename T> static inline
        T one(const T x) {
            return (x>0) ? 1 : -1;
        }
        YACK_PAIR_END(greatest);
       
        //______________________________________________________________________
        //
        //
        //! list of actors
        //
        //______________________________________________________________________
        class actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~actors() throw(); //!< cleanup
            explicit actors() throw(); //!< setup empty

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            
            //! append a new actor(sp,nu>0);
            void operator()(const species &sp,
                            const unsigned nu);

            //! emergency remove last
            void remove_last() throw();


            const list_of<actor> * operator->() const throw(); //!< access to const crew
            bool attached_to(const actor  &) const throw();    //!< check link with actor
            bool attached_to(const actors &) const throw();    //!< check link with other actors
            //!

            //! mass action with rmulops
            double mass_action(const readable<double> &C,
                               rmulops                &ops) const;

            
            //! mass action with rmulops, C+nu*xi >=0
            double mass_action(const readable<double> &C,
                               const double            xi,
                               rmulops                &ops) const;

            //! C -> max_of(C+nu*xi,0) (sign of xi must be adapted)
            void  move(writable<double> &C, const double xi) const throw();

            //! C -> C+nu*xi, raw value, sign of xi must be adapted
            void  mov_(writable<double> &C, const double xi) const throw();


            const xlimit     *genuine_limit(xlimit::field &xl, const readable<double> &C) const throw(); //!< test all actors

            
            //! display
            YACK_PROTO_OSTREAM(actors);

            //! to string
            string to_string() const;

            //! check that any of the acting concentration is negative
            bool are_genuinely_blocked_by(const readable<double> &C) const throw();

            //! check that any of the primary acting concentration is negative
            bool are_primarily_blocked_by(const readable<double> &C) const throw();

            //! compute derivatives of mass action w.r.t C, return greatest |max|
            greatest grad_action(writable<double>       &psi,
                                 const double            factor,
                                 const readable<double> &C,
                                 rmulops                &xmul) const;
            
            //! display list of compact associated concentrations
            void display_compact(std::ostream &, const readable<double> &C) const;

            //! display list of compact associated concentrations
            void display_compact(std::ostream &, const readable<double> &C, const readable<bool> &flag) const;

            //! fill array with coefficients at their position
            template <typename T> inline
            void fill( writable<T> &q ) const {
                const bare<T> _0; q.ld(*_0);
                for(const actor *a=crew.head;a;a=a->next)
                    q[***a] = a->nu;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned  molecularity; //!< molecularity = sum_i nu_i
            const int       algebraic_Z;  //!< algebraic_Z  = sum_i nu_i z_i

        protected:
            const cxx_list_of<actor>           crew; //!< actors
       
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
        };

    }

}

#endif
