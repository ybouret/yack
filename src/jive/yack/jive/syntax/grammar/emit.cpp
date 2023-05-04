#include "yack/jive/syntax/grammar.hpp"
#include "yack/jive/syntax/rule/all.hpp"
#include "yack/type/hexa.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            void grammar:: collect_keywords(sequence<string> &terminals,
                                            sequence<string> &internals) const
            {

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
                                    terminals.push_back( *(t->name) );
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
                                    internals.push_back( *(a->name) );
                                    break;
                                case group:
                                    break;
                            }
                            
                        } break;
                        default:
                            break;
                    }
                }

            }


            static inline void emit_quoted(ios::ostream &fp, const string &id)
            {
                fp << ' ' << '\"' << id << '\"';
            }


            static inline void push_define(sequence<string> &df, const string &id)
            {
                string ans;
                for(size_t i=1;i<=id.size();++i)
                {
                    const char c = id[i];

                    switch(c)
                    {
                        case ' ': ans += '_';     continue;
                        case ',': ans += "COMMA"; continue;
                        default:
                            break;
                    }

                    if(isalnum(c)||'_'==c)
                    {
                        ans += c;
                        continue;
                    }

                    ans += 'x';
                    ans += hexa::uppercase_text[ uint8_t(c) ];
                    
                }

                df.push_back(ans);
            }

            static inline void output_sepline(ios::ostream &fp)
            {
                fp << '/' << '/';
                for(size_t i=0;i<32;++i) fp << '-';
                fp << '\n';
            }

            void grammar:: emit_keywords(ios::ostream           &fp,
                                         const readable<string> &table,
                                         const string           &cxx_id,
                                         const string           &prefix)
            {
                const size_t   n = table.size();
                vector<string> df(n,as_capacity);
                size_t         dw = 0;

                output_sepline(fp);
                fp << "// keywords\n";
                output_sepline(fp);
                fp << "const char *" << cxx_id << "[] = {\n";
                if(n>0)
                {
                    emit_quoted(fp,table[1]);
                    push_define(df,table[1]);
                    dw = df.back().size();
                    for(size_t i=2;i<=n;++i)
                    {
                        fp << ',' << '\n';
                        emit_quoted(fp,table[i]);
                        push_define(df,table[i]);
                        dw = max_of(dw,df.back().size());
                    }
                    fp << '\n';
                }
                fp << "};\n\n";
                fp << "// (" << cxx_id << ',' << "sizeof(" << cxx_id << ")/sizeof(" << cxx_id << "[0]))\n\n";

                output_sepline(fp);
                fp << "// defines\n";
                output_sepline(fp);
                unsigned j = 0;
                for(size_t i=1;i<=n;++i,++j)
                {
                    const string &def = df[i];
                    fp << "#define " << prefix << def;
                    for(size_t k=def.size();k<dw;++k) fp << ' ';
                    fp(" %4u ",j);
                    fp << "//!< \"" << table[i] << "\"\n";
                }
                fp << '\n';

            }

        }

    }
    
}

