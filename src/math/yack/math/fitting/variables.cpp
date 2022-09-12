#include "yack/math/fitting/variables.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/ops.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {


            const char variables:: clid[] = "variables";

            variables:: ~variables() throw()
            {
                
            }

            variables:: variables() throw():
            large_object(), counted(),
            vdb(),
            pdb(),
            nlen(0)
            {
                
            }


            size_t variables:: size() const throw()
            {
                return (*vdb.tree).size;
            }

            size_t variables:: lower() const throw()
            {
                const vnode *node = (*vdb.tree).head;
                if(node)
                {
                    return ****node;
                }
                else
                {
                    return 0;
                }
            }

            size_t variables:: upper() const throw()
            {
                const vnode *node = (*vdb.tree).tail;
                if(node)
                {
                    return ****node;
                }
                else
                {
                    return 0;
                }
            }

            const vnode * variables:: head() const throw()
            {
                return (*vdb.tree).head;
            }

            std::ostream & operator<<(std::ostream &os, const variables &vars)
            {
                os << '[' << vars.lower() << ':' << vars.upper() << ']' << vars.vdb;
                return os;
            }


            const variable & variables:: fetch(const string &uuid) const
            {
                const variable::pointer * ppv = vdb.search(uuid);
                if(!ppv) throw imported::exception(clid,"no variable '%s'", uuid());
                return **ppv;

            }

            const variable & variables:: fetch(const char *uuid) const
            {
                const string _(uuid);
                return fetch(_);
            }




            template <typename NODE> static inline
            int compare_indices_of(const NODE *lhs, const NODE *rhs) throw()
            {
                return comparison::increasing(****lhs,****rhs);

            }





            const variable & variables:: operator()(const string &name,
                                                    const size_t indx)
            {
                // check index
                if(indx<=0) throw imported::exception(clid,"try to set '%s' at index=0", name());

                // check no multiple index
                for(const pnode *node= (*pdb.tree).head;node;node=node->next)
                {
                    const primary &q = ***node;
                    if(*q==indx) throw imported::exception(clid,"try to set index=%u for '%s', is already used by '%s'", unsigned(indx), name(), q.name() );
                }

                // try to insert new primary
                const primary::handle p = new primary(name,indx);
                if(!pdb.insert(p)) throw imported::exception(clid,"multiple primary '%s'", name());

                // try to make it a variable
                try {
                    const variable::pointer v = & *p;
                    if(!vdb.insert(v)) throw imported::exception(clid,"multiple variable '%s'", name());
                }
                catch(...)
                {
                    (void) pdb.remove(name);
                    throw;
                }

                // update
                coerce(nlen) = max_of(nlen,name.size());
                merge_list_of<pnode>::sort( coerce((*pdb.tree)), compare_indices_of<pnode>);
                merge_list_of<vnode>::sort( coerce((*vdb.tree)), compare_indices_of<vnode>);

                // done
                return *p;
            }

            const variable & variables::operator()(const char   *name, const size_t indx)
            {
                const string _(name);
                return (*this)(_,indx);
            }



            variables & variables:: operator<<(const string &source)
            {
                variables      &self = *this;
                vector<string>  vars; tokenizer::split_with(':', vars, source);

                for(size_t i=1;i<=vars.size();++i)
                {
                    string &name = vars[i];
                    strops::strip_with(" \t", 2, name);
                    (void) self(name,upper()+1);
                }

                return self;
            }

            variables & variables:: operator<<(const char   *vars)
            {
                const string _(vars);
                return (*this) << _;
            }


        }

    }

}
