
//! \file

#ifndef YACK_FITTING_VARIABLES_INCLUDED
#define YACK_FITTING_VARIABLES_INCLUDED 1


#include "yack/math/fitting/variable/primary.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            // aliases to handle variables
            //
            //__________________________________________________________________
            typedef suffix_set<string,const primary::handle>   pset;  //!< alias
            typedef pset::knot_type                            pnode; //!< alias
            typedef suffix_set<string,const variable::pointer> vset;  //!< alias
            typedef vset::knot_type                            vnode; //!< alias


            //__________________________________________________________________
            //
            //
            //! database of variables (primary+replica)
            //
            //__________________________________________________________________
            class variables
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
                explicit variables() throw();             //!< setup empty
                virtual ~variables() throw();             //!< cleanup
                variables(const variables &);             //!< copy
                variables & operator=(const variables &); //!< assign by copy/swap

                //______________________________________________________________
                //
                // modifying methods
                //______________________________________________________________
                const variable &use(const string &name, const size_t indx); //!< new primary variable name+indx
                const variable &use(const char   *name, const size_t indx); //!< new primary variable name+indx


                variables & operator<<(const string &vars); //!< automatic adding
                variables & operator<<(const char   *vars); //!< automatic adding

                //! aliasing name to source[alias]
                const variable & operator()(const string    &name,
                                            const variables &source,
                                            const string    &alias);

                //! aliasing name to source[alias]
                const variable & operator()(const char      *name,
                                            const variables &source,
                                            const char      *alias);

                //! automatic aliasing
                template <typename UUID>
                const variable & operator()(const UUID &uuid, const variables &source)
                {
                    return (*this)(uuid,source,uuid);
                }



                //______________________________________________________________
                //
                // accessing methods
                //______________________________________________________________
                size_t       size()  const throw(); //!< number of variables
                const vnode *head()  const throw(); //!< first variables
                size_t       lower() const throw(); //!< lower index or 0
                size_t       upper() const throw(); //!< upper index of 0

                //! return registered variables
                template <typename UUID> inline
                const variable & operator[](const UUID &uuid) const
                {
                    return fetch(uuid);
                }

                //! access array named value
                template <typename ARRAY, typename ID>
                typename ARRAY::type & operator()(ARRAY &arr, const ID &id) const
                {
                    return (*this)[id](arr);
                }

                //! access array named value
                template <typename ARRAY, typename ID>
                typename ARRAY::const_type & operator()(const ARRAY &arr, const ID &id) const
                {
                    return (*this)[id](arr);
                }

                //! display with boundaries
                friend std::ostream & operator<<(std::ostream &, const variables &vars);

                //! pad name.size() to len
                std::ostream & pad(std::ostream &os, const string &name) const;

                //! display with prefix for each variable
                template <typename ARRAY> inline
                std::ostream & operator()(std::ostream &os, ARRAY &arr, const char *prefix) const
                {
                    os << '{' << std::endl;
                    for(const vnode *node=head();node;node=node->next)
                    {
                        const variable &v = ***node;
                        os << "  ";
                        if(prefix) os << prefix;
                        const string &id = v.name;
                        pad(os << id,id) << " = " << arr[*v];
                        os << std::endl;
                    }
                    os << '}';
                    return os;
                }
                

                //! transfer involved values
                template <typename TARGET, typename SOURCE> inline
                void mov(TARGET &target, SOURCE &source) const
                {
                    for(const vnode *node=head();node;node=node->next)
                    {
                        const size_t i = ****node;
                        target[i] = source[i];
                    }
                }

                //! transfer involved values
                template <typename TARGET, typename LHS, typename RHS> inline
                void sub(TARGET &AB, LHS &A, RHS &B) const
                {
                    for(const vnode *node=head();node;node=node->next)
                    {
                        const size_t i = ****node;
                        AB[i] = B[i] - A[i];
                    }
                }

                //! zero involved values
                template <typename TARGET> inline
                void ldz(TARGET &target) const
                {
                    for(const vnode *node=head();node;node=node->next)
                    {
                        const size_t i = ****node;
                        target[i] = 0;
                    }
                }

                //! load involved values
                template <typename TARGET> inline
                void ld(TARGET &target, typename TARGET::param_type value) const
                {
                    for(const vnode *node=head();node;node=node->next)
                    {
                        const size_t i = ****node;
                        target[i] = value;
                    }
                }


                //! count used variables
                size_t count(const readable<bool> &used) const throw();


                
            private:
                vset vdb;
                pset pdb;

                const variable &fetch(const string &) const;
                const variable &fetch(const char   *) const;
                void            update_with(const string &) throw();

            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t len; //!< max name length

                //! set named flags to boolean value
                variables & flags(writable<bool> &used, const string &names, const bool flag);

                //! set named flags to boolean value
                variables & flags(writable<bool> &used, const char   *names, const bool flag);

                //! set named flags to TRUE
                template <typename NAMES> inline
                variables & on(writable<bool> &used, const NAMES &names)
                {
                    return flags(used,names,true);
                }

                //! set named flags to FALSE
                template <typename NAMES> inline
                variables & off(writable<bool> &used, const NAMES &names)
                {
                    return flags(used,names,false);
                }

                //! set all named flags to TRUE, other to FALSE
                template <typename NAMES> inline
                variables & only_on(writable<bool> &used, const NAMES &names)
                {
                    ld(used,false);
                    return flags(used,names,true);
                }

            };



        }


    }

}

#endif
