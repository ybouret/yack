
#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/all.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            struct visiting
            {
                static inline void a_rule(addrbook &db, const rule *r)
                {
                    assert(r);
                    const uint32_t uuid = r->uuid;
                    switch(uuid)
                    {
                        case terminal::mark:
                            db.ensure(r);
                            break;

                        case option::mark:
                            a_wildcard(db, r->as<option>() );
                            break;

                        case repeat::mark:
                            a_wildcard(db,r->as<repeat>() );
                            break;

                        case aggregate::mark:
                            a_compound(db,r->as<aggregate>());
                            break;

                        case alternate::mark:
                            a_compound(db,r->as<alternate>());
                            break;
                    }
                }

                static inline void a_wildcard(addrbook &db, const wildcard *r)
                {
                    assert(r);
                    if(db.insert( static_cast<const rule*>(r) ) )
                    {
                        // first time visiting
                        a_rule(db, & **r);
                    }
                }

                static inline void a_compound(addrbook &db, const compound *r)
                {
                    assert(r);
                    if(db.insert(static_cast<const rule*>(r)))
                    {
                        // first time visiting
                        for(const component *chld=r->head;chld;chld=chld->next)
                        {
                            a_rule(db,& **chld);
                        }
                    }

                }
            };


            static inline void check_not_empty(const compound *r, const char *gid)
            {
                if(r->size<=0) throw exception("[%s] empty <%s>", gid, (*(r->name))() );
            }

            void grammar:: validate() const
            {
                if(lexical::scanner::verbose)
                {
                    std::cerr << "[" << lang << "] validating..." << std::endl;
                }
                const char *id = (*lang)();
                if(rules.size<=0) throw exception("[%s] has no rules",id);

                {
                    addrbook visited;
                    unsigned orphans = 0;
                    visiting::a_rule(visited,rules.head);

                    for(const rule *r=rules.head;r;r=r->next)
                    {
                        const bool used = visited.search(r);
                        if(used)
                        {
                            if(lexical::scanner::verbose) std::cerr << "\tin use";
                        }
                        else
                        {
                            if(lexical::scanner::verbose) std::cerr << "\torphan";
                            ++orphans;
                        }
                        if(lexical::scanner::verbose) std::cerr << " <" << r->name << ">" << std::endl;

                        switch(r->uuid)
                        {
                            case alternate::mark: check_not_empty(r->as<alternate>(),id); break;
                            case aggregate::mark: check_not_empty(r->as<aggregate>(),id); break;
                            default:
                                break;
                        }

                    }
                    if(orphans>0)
                    {
                        throw exception("[%s] has #orphan=%u",id,orphans);
                    }
                }



                if(lexical::scanner::verbose)
                {
                    std::cerr << "[" << lang << "] seems ok!" << std::endl;
                }
            }

        }

    }
}
