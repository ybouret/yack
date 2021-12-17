
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        
        namespace lexical
        {
            
            
            
            
            analyzer:: ~analyzer() throw()
            {
                sdb.release();
                assert(quantity()>0);
                liberate();
            }
            
            const analyzer::scan_set & analyzer:: operator*() const throw()
            {
                return sdb;
            }
            
            
            void   analyzer:: setup()
            {
                withhold();
                const scan_ptr tmp = this;
                if(!sdb.insert(tmp)) throw exception("<%s> unexpected self insertion", (*label)());
                linked_to(*this);
            }
            
            
            void analyzer:: declare(scanner *s)
            {
                const scan_ptr p(s);
                if(!sdb.insert(p)) throw exception("<%s> has multiple <%s>", (*label)(), (*(p->label))() );
                s->linked_to(*this);
            }
            
            void scanner:: linked_to(analyzer &parent) throw()
            {
                if(verbose) std::cerr << "<" << label << "> -> <" << parent.label << ">" << std::endl;
                dict_   = & parent.dict;
                ctrl_   = & parent;
                verbose = parent.verbose;
            }
            
            
            scanner * analyzer:: request(const string &target,
                                         const char   *when) const
            {
                assert(when);
                const scan_ptr *temp = sdb.search(target);
                if(!temp) throw exception("<%s> no <%s> to %s!", (*label)(), target(), when );
                return &coerce(**temp);
            }
            
            void analyzer:: impl_jump(const string &target)
            {
                scanner *dest = request(target,"jump to"); assert(dest);
                if(verbose) std::cerr << "jump <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;
                scan = dest;
            }
            
            void analyzer:: impl_call(const string &target)
            {
                scanner *dest = request(target,"call"); assert(dest);
                if(verbose) std::cerr << "jump <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;                hist.push(scan);
                hist.push_back(scan);
                scan = dest;
            }
            
            namespace flow
            {
                behavior jump:: operator()(token &word)
                {
                    cb(word);
                    az.impl_jump(*to);
                    return control;
                }
                
            }
            void analyzer:: store(lexeme *lx) throw()
            {
                assert(NULL!=lx);
                repo.push_front(lx);
            }
            
            lexeme * analyzer:: query(source &src)
            {
            QUERY:
                if(repo.size)
                {
                    return repo.pop_front();
                }
                else
                {
                    bool    ctrl = false;
                    lexeme *lx   = scan->probe(src,ctrl);
                    if(lx)
                    {
                        return lx;
                    }
                    else
                    {
                        assert(NULL==lx);
                        if(ctrl)
                        {
                            // something happened, probe again
                            goto QUERY;
                        }
                        else
                        {
                            // EOF
                            return NULL;
                        }
                    }
                }
            }
            
            
        }
        
    }
    
}

