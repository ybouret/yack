
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"
#include "yack/type/temporary.hpp"
#include "yack/ios/fmt/align.hpp"

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

            void analyzer:: reset() throw()
            {
                scan = this;
                repo.release();
                hist.release();
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

            const plugin & analyzer::import( plugin *plg )
            {
                assert(NULL!=plg);
                plugin &p = decl<plugin>(plg);
                call(p.label,p.trigger,&p,&plugin::on_call);
                return *plg;
            }


            
            void scanner:: link_to(analyzer &parent) throw()
            {
                if(verbose)
                {
                    const string id = '<' + *label + '>';
                    std::cerr << ios::align(id,output_width) << " [link to <" << parent.label << ">]" << std::endl;
                }
                dict    = & parent.dict;
                root    = & parent;
            }
            
            void scanner:: restore(token &word) throw()
            {
                assert(flux);
                flux->store(word);
            }
            
            scanner * analyzer:: request(const string &target,
                                         const char   *when) const
            {
                assert(when);
                const scan_ptr *temp = sdb.search(target);
                if(!temp) throw exception("<%s> no <%s> to %s!", (*label)(), target(), when );
                return &coerce(**temp);
            }
            
            void analyzer:: flow_jump(const string &target)
            {
                scanner *dest = request(target,"jump to"); assert(dest);
                if(verbose)
                {
                    std::cerr << "jump <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;
                }
                scan = dest;
            }
            
            void analyzer:: flow_call(const string &target)
            {
                scanner *dest = request(target,"call"); assert(dest);
                if(verbose)
                {
                    std::cerr << "call <" << scan->label << "> ==> <" << dest->label << ">" << std::endl;                hist.push(scan);
                }
                hist.push(scan);
                scan = dest;
            }
            
            void analyzer:: flow_back()
            {
                if(hist.size()<=0) throw exception("<%s> cannot return", (*(scan->label))() );
                if(verbose)
                {
                    std::cerr << "<" << scan->label << "> back to <" << hist.peek()->label << ">" << std::endl;
                }
                scan = hist.pop();
            }
            
          
            
            void analyzer:: store(lexeme *lx) throw()
            {
                assert(NULL!=lx);
                repo.push_front(lx);
            }
            
            lexeme * analyzer:: query(source &src)
            {
                const temporary<source *> assign(flux,&src);
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
                        // produce lexeme
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

