#include "yack/jive/lexical/scanner.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }


            void scanner:: make(const tag &t, const motif &m, const action &a)
            {
                if(verbose)
                {
                    const string xp = m->express();
                    std::cerr << "<" << label << "> (*) " << t << " <-- '" << xp << "'" << std::endl;
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
                return produce;
            }


            void scanner:: check_token(const tag &uuid, const token &word, const context &whom) const
            {
                if(word.size<=0)
                {
                    exception excp("unexpected empty token '%s' for <%s>", (*uuid)(), (*label)() );
                    throw whom.stamp(excp);
                }
            }



            lexeme * scanner:: probe(source &src)
            {
                const character *ch = src.peek();
                if(!ch)
                {
                    //----------------------------------------------------------
                    //
                    // eof
                    //
                    //----------------------------------------------------------
                    if(verbose) std::cerr << "<" << label << "> EOF" << std::endl;
                    return NULL; // EOF
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // initialize search of matchin pattern starting with char
                    //
                    //----------------------------------------------------------
                    size_t               left = 0;
                    const scatter::node *node = table->query(**ch,left);
                    const directive     *best = 0;
                    token                word;
                    while(left>0)
                    {
                        --left;
                        best = static_cast<const directive *>(**node); assert(best);
                        if(best->info->accept(src,word))
                        {
                            if(verbose) std::cerr << "<" << label << "> init '" << best->uuid << "' = '" << word << "'" << std::endl;
                            check_token(best->uuid,word,*src);
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
                    // look for better matches
                    //
                    //----------------------------------------------------------
                    //if(verbose) std::cerr << "<" << label << "> remaining directives: " << left << std::endl;
                    if(left>0)
                    {
                        assert(node);          // of best directive
                        src.store_copy(word);  // restore source, keeping current word

                        while(NULL!=(node=node->next))
                        {
                            const directive *deed = static_cast<const directive *>(**node); assert(deed);
                            token            temp;
                            if(deed->info->accept(src,temp))
                            {
                                check_token(deed->uuid,temp,*src);
                                if(temp.size>word.size)
                                {
                                    //------------------------------------------
                                    // new winner
                                    //------------------------------------------
                                    src.store_copy(temp);
                                    word.swap_with(temp);
                                    best = deed;
                                    if(verbose) std::cerr << "<" << label << "> keep '" << best->uuid << "' = '" << word << "'" << std::endl;
                                }
                                else
                                {
                                    //------------------------------------------
                                    // too late, return to source
                                    //------------------------------------------
                                    src.store(temp);
                                }
                            }
                            else
                            {
                                assert(0==temp.size);
                            }
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // found a matching rule
                    //
                    //----------------------------------------------------------
                    if(verbose) std::cerr << "<" << label << "> scan '" << best->uuid << "' = '" << word << "'" << std::endl;
                    src.skip( word.size );
                    

                    return NULL;
                }

            }


        }
        

    }
}
