
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            //const char regulator:: jump_[] = ".jump.";

            regulator:: ~regulator() throw() {}

            regulator:: regulator(const tag      &n,
                                  const callback &p,
                                  analyzer       &c) :
            name(n),
            proc(p),
            ctrl(c)
            {
            }

            regulator:: regulator(const regulator &other) :
            name(other.name),
            proc(other.proc),
            ctrl(other.ctrl)
            {
            }


            behavior jump:: operator()(token &word)
            {
                coerce(proc)(word);
                ctrl.do_jump(*name);
                return control;
            }



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
                dict_ = & parent.dict;
                ctrl_ = & parent;
            }


            void analyzer:: do_jump(const string &target)
            {
                std::cerr << "jump <" << label << "> ==> <" << target << ">" << std::endl;
                const scan_ptr *temp = sdb.search(target);
                if(!temp) throw exception("<%s> no <%s> to jump to!", (*label)(), target() );
                scan = &coerce(**temp);
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

