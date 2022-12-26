//! \file

#ifndef YACK_CHEMICAL_EQ_INCLUDED
#define YACK_CHEMICAL_EQ_INCLUDED 1

#include "yack/chem/components.hpp"
#include "yack/data/list/meta-repo.hpp"

namespace yack
{
    namespace chemical
    {
        
       
        //______________________________________________________________________
        //
        //
        //! equilibrium = entity + components, with a K(time)
        //
        //______________________________________________________________________
        class equilibrium : public entity, public components
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> pointer; //!< alias
           
            
            static double display_time; //!< default=0
            static size_t display_size; //!< default=12


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            double             K(double)               const;                                 //!< non-virtual, checkin getK()
            const components & content()               const throw();                         //!< cast
            friend             std::ostream & operator<<(std::ostream &, const equilibrium&); //!< display
            void               vizlink(ios::ostream &, const library &) const;                //!< link to registered species

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() throw();  //!< cleanup
            
        protected:
            //! set from name + index
            template <typename NAME> inline
            explicit equilibrium(const NAME &uid, const size_t idx) :
            entity(uid,idx), components()
            {
            }
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual double      getK(double) const = 0;
            virtual const char *attr()       const;
        };

        typedef meta_list<const equilibrium> eq_team_; //!< alias
        typedef meta_node<const equilibrium> eq_node;  //!< alias
        typedef meta_repo<const equilibrium> eq_repo;  //!< alias

        //! team of equilibria
        class eq_team : public object, public eq_team_
        {
        public:
            explicit eq_team() throw(); //!< setup
            virtual ~eq_team() throw(); //!< cleanup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_team);
        };

        class eq_squad : public eq_team
        {
        public:
            explicit eq_squad() throw() : eq_team(), next(0), prev(0) {}
            virtual ~eq_squad() throw() {}
            
            
            eq_squad *next;
            eq_squad *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_squad);
        };
        
        class eq_squads : public cxx_list_of<eq_squad>
        {
        public:
            explicit eq_squads() throw();
            virtual ~eq_squads() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_squads);
        };
        
        
    }
    
}

#endif
