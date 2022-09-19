#include "yack/math/fitting/variables.hpp"
#include "yack/math/fitting/variable/replica.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"

#include "yack/data/list/sort.hpp"
#include "yack/sequence/vector.hpp"

#include "yack/string/ops.hpp"
#include "yack/string/tokenizer.hpp"

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
            vdb(),
            pdb(),
            len(0)
            {
                
            }

            variables:: variables(const variables &other) :
            vdb(other.vdb),
            pdb(other.pdb),
            len(other.len)
            {

            }

            variables & variables:: operator=(const variables &other)
            {
                variables temp(other);

                vdb.exchange_content_with(temp.vdb);
                pdb.exchange_content_with(temp.pdb);
                coerce_cswap(len,temp.len);

                return *this;
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





            const variable & variables:: use(const string &name,
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
                update_with(name);

                // done
                return *p;
            }

            void variables:: update_with(const string &name) throw()
            {
                coerce(len) = max_of(len,name.size());
                merge_list_of<pnode>::sort( coerce((*pdb.tree)), compare_indices_of<pnode>);
                merge_list_of<vnode>::sort( coerce((*vdb.tree)), compare_indices_of<vnode>);
            }


            const variable & variables::use(const char   *name, const size_t indx)
            {
                const string _(name);
                return use(_,indx);
            }

            const variable & variables:: operator()(const string    &name,
                                                    const variables &source,
                                                    const string    &alias)
            {
                const primary::handle *ppp = source.pdb.search(alias);
                if(!ppp) throw imported::exception(clid,"no primary '%s' in source",alias());
                const variable::pointer v = new replica(name,*ppp);
                if(!vdb.insert(v)) throw imported::exception(clid,"multiple '%s' while aliasing '%s'",name(),alias());
                update_with(name);
                return *v;
            }

            const variable & variables :: operator()(const char      *name,
                                                     const variables &source,
                                                     const char      *alias)
            {
                const string _(name);
                const string __(alias);
                return(*this)(_,source,__);
            }



            variables & variables:: operator<<(const string &source)
            {
                variables      &self = *this;
                vector<string>  vars; tokenizer::split_with(':', vars, source);

                for(size_t i=1;i<=vars.size();++i)
                {
                    string &name = vars[i];
                    strops::strip_with(" \t", 2, name);
                    (void) use(name,upper()+1);
                }

                return self;
            }

            variables & variables:: operator<<(const char   *vars)
            {
                const string _(vars);
                return (*this) << _;
            }

            std::ostream & variables:: pad(std::ostream &os, const string &name) const
            {

                for(size_t i=name.size();i<len;++i) os << ' ';
                return os;
            }

            size_t variables:: count(const readable<bool> &used) const throw()
            {
                size_t res = 0;
                for(const vnode *node=head();node;node=node->next)
                {
                    if( used[ ****node ] ) ++res;
                }
                return res;
            }

        }

    }

}
