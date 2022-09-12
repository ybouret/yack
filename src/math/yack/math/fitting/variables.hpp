
//! \file

#ifndef YACK_FITTING_VARIABLES_INCLUDED
#define YACK_FITTING_VARIABLES_INCLUDED 1


#include "yack/math/fitting/replica.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            typedef suffix_set<string,const primary::handle>   pset;
            typedef pset::knot_type                            pnode;
            typedef suffix_set<string,const variable::pointer> vset;
            typedef vset::knot_type                            vnode;


            class variables :
            public large_object,
            public counted
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const char clid[]; //!< "variables"

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit variables() throw();
                virtual ~variables() throw();


                //______________________________________________________________
                //
                // modifying methods
                //______________________________________________________________
                void operator()(const string &name, const size_t indx);
                void operator()(const char   *name, const size_t indx);

                template <typename UUID> inline
                variables & operator<<(const UUID &uuid)
                {
                    variables &self = *this;
                    self(uuid,size()+1);
                    return self;
                }

                //______________________________________________________________
                //
                // accessing methods
                //______________________________________________________________
                size_t       size()  const throw();
                const vnode *head()  const throw();
                size_t       lower() const throw();
                size_t       upper() const throw();

                template <typename UUID> inline
                const variable & operator[](const UUID &uuid) const
                {
                    return fetch(uuid);
                }

                friend std::ostream & operator<<(std::ostream &, const variables &vars);


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(variables);
                vset vdb;
                pset pdb;

                const variable &fetch(const string &) const;
                const variable &fetch(const char   *) const;

            public:
                const size_t nlen; //!< max name length
                
            };


            typedef arc_ptr<variables> shared_vars;

        }


    }

}

#endif
