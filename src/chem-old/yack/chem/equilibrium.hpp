//! \file

#ifndef YACK_CHEMICAL_EQUILIBRIUM_INCLUDED
#define YACK_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/eqs/components.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! equilibrium = components + entity + constant(time)
        //
        //______________________________________________________________________
        class equilibrium : public components, public entity, public ios::vizible
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> ptr; //!< shared pointer
            static  double t_display;                //!< global display time
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual equilibrium *clone() const = 0;          //!< for virtual copy
            double               K(double);                  //!< check getK(t) > 0
            YACK_PROTO_OSTREAM(equilibrium);                 //!< display name


            std::ostream &display(std::ostream &, const size_t length = 0); //!< display with padding
            void          viz(ios::ostream &, const string &ppty) const;    //!< save for GraphViz
            

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t rank; //!< 1=primary, otherwise replica of rank primaries

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() noexcept; //!< cleanup

        protected:
            //! setup empty
            template <typename NAME> inline
            explicit equilibrium(NAME        &uid,
                                 const size_t idx) :
            components(),
            entity(uid,idx),
            rank(1)
            {
            }

            //! hard copy
            equilibrium(const equilibrium &);

        private:
            YACK_DISABLE_ASSIGN(equilibrium);
            virtual double getK(double) = 0;
        };

        //______________________________________________________________________
        //
        //
        //! built-in constant equilibrium
        //
        //______________________________________________________________________
        class const_equilibrium : public equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit const_equilibrium(NAME &uid, const double K0, const size_t idx) :
            equilibrium(uid,idx),
            K_(K0)
            {
            }
            const_equilibrium(const const_equilibrium &); //!< copy
            virtual ~const_equilibrium() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual equilibrium *clone() const; //!< new const_equilibrium(*this)

        private:
            YACK_DISABLE_ASSIGN(const_equilibrium);
            const double K_;
            virtual double getK(double) noexcept;
        };

        typedef core_repo<const equilibrium>  eq_list;  //!< simple list
        typedef coop_repo<const equilibrium>  eq_repo;  //!< list with cache
        typedef eq_list::node_type            eq_node;  //!< common node
        typedef eq_repo::proxy_type           eq_proxy; //!< alias
        typedef eq_repo::zpool_type           eq_zpool; //!< alias

    }

}

#endif