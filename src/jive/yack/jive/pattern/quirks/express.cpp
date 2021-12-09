#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {

        namespace {

            static inline void express_c_code(string &target, const uint8_t code)
            {
                bool use_hexa = true;
                if(isalnum(code))
                {
                    use_hexa = false;
                }
                else
                {
                    switch(code)
                    {
                        case ' ':
                        case '_':
                        case '@':
                        case ',':
                        case ';':
                        case '<':
                        case '=':
                        case '>':
                            use_hexa = false;
                        default:
                            break;
                    }
                }
                if(use_hexa)
                {
                    target += vformat("\\x%02x",code);
                }
                else
                {
                    target += char(code);
                }

            }

            template <typename T>
            void express_as(string &, const pattern *);

            template <>
            inline void express_as<single>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<single>());
                express_c_code(target,p->as<single>()->code);
            }

            template <>
            inline void express_as<within>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<within>());
                const within &w = *(p->as<within>());
                target += '[';
                express_c_code(target,w.lower);
                target += '-';
                express_c_code(target,w.upper);
                target += ']';
            }

            template <>
            inline void express_as<except>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<except>());
                target += '[';
                target += '^';
                express_c_code(target,p->as<except>()->code);
                target += ']';
            }



            template <>
            inline void express_as<optional>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<optional>());
                const optional &jk = *(p->as<optional>());
                pattern::express(target,& *jk);
                target += '?';
            }

            template <>
            inline void express_as<at_least>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<at_least>());
                const at_least &jk = *(p->as<at_least>());
                pattern::express(target,& *jk);
                switch(jk.count)
                {
                    case 0:  target += '*'; break;
                    case 1:  target += '+'; break;
                    default: target += vformat("{%u}", unsigned(jk.count) );
                        break;
                }
            }

            template <>
            inline void express_as<counting>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<counting>());
                const counting &jk = *(p->as<counting>());
                pattern::express(target,& *jk);
                target += vformat("{%u,%u}", unsigned(jk.nmin), unsigned(jk.nmax) );
            }



            template <>
            inline void express_as<op_and>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<op_and>());
                const op_and &op = *(p->as<op_and>());
                target += '(';
                for(const pattern *m=op.head;m;m=m->next)
                {
                    pattern::express(target,m);
                }
                target += ')';
            }

            template <>
            inline void express_as<op_or>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<op_or>());
                const op_or &op = *(p->as<op_or>());
                target += '(';
                const pattern *m=op.head;
                if(m)
                {
                    pattern::express(target,m);
                    while(NULL!=(m=m->next))
                    {
                        target += '|';
                        pattern::express(target,m);
                    }
                }

                target += ')';
            }

            template <>
            inline void express_as<op_none>(string &target, const pattern *p)
            {
                assert(p); assert(p->is<op_none>());
                const op_none &op = *(p->as<op_none>());
                target += '(';
                const pattern *m=op.head;
                if(m)
                {
                    pattern::express(target,m);
                    while(NULL!=(m=m->next))
                    {
                        target += '|';
                        pattern::express(target,m);
                    }
                }

                target += ')';
                target += '!';
            }


        }




        void pattern:: express(string        &target,
                               const pattern *p)
        {
            assert(NULL!=p);
            switch(p->uuid)
            {
                case single::mark: express_as<single>(target,p); break;
                case within::mark: express_as<within>(target,p); break;
                case except::mark: express_as<except>(target,p); break;

                case optional::mark: express_as<optional>(target,p); break;
                case at_least::mark: express_as<at_least>(target,p); break;
                case counting::mark: express_as<counting>(target,p); break;

                case op_and::mark:  express_as<op_and>(target,p);  break;
                case op_or::mark:   express_as<op_or>(target,p);   break;
                case op_none::mark: express_as<op_none>(target,p); break;


                default:
                    throw exception("pattern::express(unhandled [%s])", yack_fourcc(p->uuid));
            }

        }

        string pattern::express() const
        {
            string target;
            express(target,this);
            return target;
        }

    }

}
