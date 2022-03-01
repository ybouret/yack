#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/all.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            void grammar:: emit_keywords(ios::ostream &fp) const
            {
                vector<string> kw;
                
                for(const rule *p = rules.head;p;p=p->next)
                {
                    switch(p->uuid)
                    {
                        case terminal::mark: {
                           
                            const terminal *t = p->as<terminal>();
                            switch(t->role)
                            {
                                case standard:
                                case univocal:
                                    kw.push_back( *(t->name) );
                                    break;
                                case division:
                                    break;
                            }
                            
                        } break;
                            
                        case aggregate::mark: {
                            
                            const aggregate *a = p->as<aggregate>();
                            switch(a->role)
                            {
                                case named:
                                case proxy:
                                    kw.push_back( *(a->name) );
                                    break;
                                case group:
                                    break;
                            }
                            
                        } break;
                        default:
                            break;
                    }
                }
                
                const size_t n = kw.size();
                if(n>0)
                {
                    fp << '\"' << kw[1] << '\"';
                    for(size_t i=2;i<=n;++i)
                    {
                        fp << ',' << '\n';
                        fp << '\"' << kw[i] << '\"';
                    }
                    fp << '\n';
                }
                
            }
        }
        
    }
    
}

