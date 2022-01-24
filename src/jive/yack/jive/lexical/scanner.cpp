#include "yack/jive/lexical/scanner.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/ios/fmt/align.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            bool   scanner:: verbose = false;
            size_t scanner:: output_width = 20;

            scanner:: ~scanner() throw()
            {
            }


            const string & scanner:: key() const throw()
            {
                return *label;
            }

            const string scanner:: sid() const
            {
                return '<' + *label + '>';
            }
            

            void scanner:: on(const tag &t, const motif &m, const action &a)
            {
                if(verbose)
                {
                    const string xp = m->express();
                    const string id = '<' + *label + '>';
                    std::cerr << ios::align(id,output_width);
                    const string ts = '@' + *t + '@';
                    std::cerr << ' ' << ios::align(ts,output_width);
                    std::cerr << " : '" << xp << "'" << std::endl;
                }

                if(m->feeble()) throw exception("<%s> using feeble pattern!", (*label)() );

                const instruction task = new directive(t,m,a);
                if(!coerce(*instr).insert(task)) throw exception("<%s> multiple directive '%s'", (*label)(), (*t)());
                coerce(*table).store(*m,& *task);
            }

            behavior scanner:: on_produce(const token &) throw()
            {
                return produce;
            }

            behavior scanner:: on_discard(const token &) throw()
            {
                return discard;
            }

            behavior scanner:: on_newline(const token &) throw()
            {
                assert(flux);
                (**flux).newline();
                return discard;
            }

            void scanner:: check_word(const tag &uuid, const token &word) const
            {
                assert(NULL!=flux);
                if(word.size<=0)
                {
                    exception excp("unexpected empty token '%s' for <%s>", (*uuid)(), (*label)() );
                    throw (**flux).stamp(excp);
                }
            }

            void scanner:: check_root() const
            {
                if(!root) throw exception("<%s> is not linked (branching not supported)", (*label)() );
            }
            
            std::ostream & operator<<(std::ostream &os, const scanner &s)
            {
                os << '<' << s.label << '>';
                return os;
            }

        }

    }
}

#include "yack/type/temporary.hpp"
namespace yack
{
    namespace jive
    {
        namespace lexical
        {

            bool  scanner:: standalone() const throw()
            {
                return indx == &indx_;
            }

            bool scanner:: linked_to(const scanner &host) const throw()
            {
                if( host.indx == indx )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }


            lexeme  * scanner:: newlex(const tag &uid, const context &ctx)
            {
                try
                {
                    assert(indx);
                    const size_t I = ++(*indx);
                    return new lexeme(uid,ctx,I);
                }
                catch(...)
                {
                    --(*indx);
                    throw;
                }
            }


            lexeme * scanner:: probe(source &src, bool &ctrl)
            {

                const temporary<source *> assign(flux,&src);
                ctrl = false;

            PROBE:
                const character *ch = src.peek();
                if(!ch)
                {
                    //----------------------------------------------------------
                    //
                    // eos
                    //
                    //----------------------------------------------------------
                    YACK_JIVE_LEX_PRINTLN(sid() << " EOS");
                    return NULL; // EOS
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // initialize search of matching pattern starting with char
                    //
                    //----------------------------------------------------------
                    size_t               left = 0;                       // number of possible pattern
                    const scatter::node *node = table->query(**ch,left); // get first possible node
                    const directive     *best = 0;                       // the best matching directive
                    token                word;                           // the best matching token
                    while(left>0)
                    {
                        --left; assert(node);
                        best = static_cast<const directive *>(**node); assert(best);
                        if(best->info->accept(src,word))
                        {
                            YACK_JIVE_LEX_PRINTLN(sid() << " init '" << best->name << "' = '" << word << "'");
                            check_word(best->name,word);
                            break;
                        }
                        best = NULL;
                        node = node->next;
                    }

                    //----------------------------------------------------------
                    //
                    // no best rule so far...
                    //
                    //----------------------------------------------------------
                    if(!best)
                    {
                        assert(0==word.size);
                        exception excp("unexpected '%s' for <%s>", ios::ascii::hybrid[**ch], (*label)() );
                        throw (*src).stamp(excp);
                    }

                    //----------------------------------------------------------
                    //
                    // look for better matches, with cache management
                    //
                    //----------------------------------------------------------
                    bool replaced = false;
                    if(left>0)
                    {
                        assert(node);                  // of best directive
                        src.store_copy(word);          // restore source, keeping current word
                        assert(src.read()>=word.size); // sanity check

                        // loop over next possibilities
                        while(NULL!=(node=node->next))
                        {
                            const directive *deed = static_cast<const directive *>(**node); assert(deed);
                            token            temp;

                            if(deed->info->accept(src,temp))
                            {
                                check_word(deed->name,temp);
                                if(temp.size>word.size)
                                {
                                    //------------------------------------------
                                    // new winner
                                    //------------------------------------------
                                    src.store_copy(temp);
                                    word.swap_with(temp);
                                    best = deed;
                                    YACK_JIVE_LEX_PRINTLN(sid() << " keep '" << best->name << "' = '" << word << "'");
                                    assert(src.read()>=word.size);
                                    replaced=true;
                                }
                                else
                                {
                                    //------------------------------------------
                                    // too late, return to source
                                    //------------------------------------------
                                    src.store(temp);
                                    assert(src.read()>=word.size);
                                }
                            }
                            else
                            {
                                assert(0==temp.size);
                                assert(src.read()>=word.size);
                            }
                        }
                        assert(src.read()>=word.size);

                        // update source according to the best match
                        src.skip( word.size );
                    }

                    //----------------------------------------------------------
                    //
                    // found a matching rule
                    //
                    //----------------------------------------------------------
                    if(replaced) YACK_JIVE_LEX_PRINTLN(sid() << " scan '" << best->name << "' = '" << word << "' @" << **flux << ':' << (**flux).line);

                    action &perform = coerce(best->duty);
                    switch(perform(word))
                    {
                        case produce:
                            break;

                        case discard:
                            goto PROBE;

                        case control:
                            ctrl = true;
                            return NULL;
                    }

                    lexeme *lx = newlex(best->name,*word);
                    lx->data.swap_with(word);
                    return lx;
                }

            }


        }
        

    }
}
