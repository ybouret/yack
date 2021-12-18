
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"
#include "yack/type/temporary.hpp"

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
            
            const scanner & analyzer:: operator[](const string &label) const
            {
                const scan_ptr *pps = sdb.search(label);
                if(!pps) throw exception("no scanner");
                return **pps;
            }
            
            void   analyzer:: setup()
            {
                withhold();
                const scan_ptr tmp = this;
                if(!sdb.insert(tmp)) throw exception("<%s> unexpected self insertion", (*label)());
                link_to(*this);
            }
            
            
            void analyzer:: declare(scanner *s)
            {
                const scan_ptr p(s);
                if(!sdb.insert(p)) throw exception("<%s> has multiple <%s>", (*label)(), (*(p->label))() );
                s->link_to(*this);
            }
            
            void scanner:: link_to(analyzer &parent) throw()
            {
                if(verbose) std::cerr << "<" << label << "> -> <" << parent.label << ">" << std::endl;
                dict    = & parent.dict;
                ctrl_   = & parent;
                verbose = parent.verbose;
            }
            
            void scanner:: restore(token &word) throw()
            {
                assert(root);
                root->store(word);
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
                if(verbose)
                {
                    std::cerr << "jump <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;
                }
                scan = dest;
            }
            
            void analyzer:: impl_call(const string &target)
            {
                scanner *dest = request(target,"call"); assert(dest);
                if(verbose)
                {
                    std::cerr << "call <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;                hist.push(scan);
                }
                hist.push(scan);
                scan = dest;
            }
            
            void analyzer:: impl_back()
            {
                if(hist.size()<=0) throw exception("<%s> cannot return", (*(scan->label))() );
                if(verbose)
                {
                    std::cerr << "<" << scan->label << "> back to <" << hist.peek()->label << ">" << std::endl;
                }
                scan = hist.pop();
            }
            
            namespace flow
            {
                behavior jump:: operator()(token &word)
                {
                    cb(word);
                    az.impl_jump(*to);
                    return control;
                }
                
                behavior call:: operator()(token &word)
                {
                    cb(word);
                    az.impl_call(*to);
                    return control;
                }
                
                behavior back::operator()(token &word)
                {
                    cb(word);
                    az.impl_back();
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
                const temporary<source *> assign(root,&src);
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

