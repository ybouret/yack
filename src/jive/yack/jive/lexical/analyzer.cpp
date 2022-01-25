
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
                assert(indx);
                *indx = 0;
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
                mydb = &dict;
                root = this;
            }
            
            
            void analyzer:: declare(scanner *s, const decl_t d)
            {
                assert(s);
                const scan_ptr p(s);
                if(!sdb.insert(p)) throw exception("<%s> has multiple <%s>", (*label)(), (*(p->label))() );
                switch(d)
                {
                    case declare_manual:
                        s->link_to(*this);
                        break;

                    case declare_plugin:
                        // already linked
                        assert(s->linked_to(*this));
                        break;
                }
            }

            const plugin & analyzer::import( plugin *plg )
            {
                assert(NULL!=plg);
                plugin &p = decl<plugin>(plg,declare_plugin);
                call(p.label,p.trigger,&p,&plugin::on_call);
                return *plg;
            }


            void scanner:: link_to(analyzer &parent) throw()
            {
                assert(standalone());
                YACK_JIVE_LEX_PRINTLN( ios::align(sid(),output_width) << " [ link to <" << parent.label << "> ]" );

                mydb    = & parent.dict;
                root    = & parent;
                indx    =   parent.indx;
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
                YACK_JIVE_LEX_PRINTLN("jump " << scan->sid() << " ==> " << dest->sid());
                scan = dest;
            }
            
            void analyzer:: flow_call(const string &target)
            {
                scanner *dest = request(target,"call"); assert(dest);
                YACK_JIVE_LEX_PRINTLN("call " << scan->sid() << " ==> " << dest->sid());
                hist.push(scan);
                scan = dest;
            }
            
            void analyzer:: flow_back()
            {
                if(hist.size()<=0) throw exception("<%s> cannot return", (*(scan->label))() );
                YACK_JIVE_LEX_PRINTLN(scan->sid() << "back to " << hist.peek()->sid());
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
                        //------------------------------------------------------
                        //
                        // produce lexeme
                        //
                        //------------------------------------------------------
                        return lx;
                    }
                    else
                    {
                        assert(NULL==lx);
                        if(ctrl)
                        {
                            //--------------------------------------------------
                            //
                            // something happened, probe again
                            //
                            //--------------------------------------------------
                            goto QUERY;
                        }
                        else
                        {
                            //--------------------------------------------------
                            //
                            // EOF
                            //
                            //--------------------------------------------------
                            switch(scan->onEOS)
                            {
                                case accept_eos:
                                    YACK_JIVE_LEX_PRINTLN(scan->sid() << " accept EOS");
                                    break;

                                case reject_eos: {
                                    YACK_JIVE_LEX_PRINTLN(scan->sid() << " reject EOS");
                                    exception excp("End Of Stream for <%s>", (*(scan->label))() );
                                    (**flux).stamp(excp);
                                    throw excp;
                                } break;
                            }

                            return NULL;
                        }
                    }
                }
            }


            const lexeme * analyzer:: peek(source &src)
            {
                lexeme *ans = query(src);
                if(ans)
                {
                    store(ans);
                }
                return ans;
            }

            bool analyzer:: owns(const lexeme *lx) const throw()
            {
                return repo.owns(lx);
            }


            void analyzer:: show(const char *pfx) const
            {
                if(!pfx) pfx="";

                std::cerr << pfx << "\t<" << label << " repo='" << repo.size << "'>" << std::endl;
                for(const lexeme *lx=repo.head;lx;lx=lx->next)
                {
                    std::cerr << pfx << "\t\t" << '#' << lx->indx << " : " << lx->name << " : '" << lx->data << "'" << std::endl;
                }
                std::cerr << pfx << "\t<" << label <<"/>" << std::endl;
            }

        }
        
    }
    
}

