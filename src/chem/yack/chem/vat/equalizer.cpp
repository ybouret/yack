
#include "yack/chem/vat/equalizer.hpp"


namespace yack
{
    namespace Chemical
    {

        Cursor:: Cursor(const double         c,
                        const Actor         &a,
                        const Species::Fund &f) :
        Species::CoopRepo(f),
        xi( Extended::Send(c) / a.xn )
        {
            (*this) << *a;
        }


        Cursor:: ~Cursor() noexcept
        {
        }

    }

}

#include "yack/exception.hpp"

namespace yack
{
    namespace Chemical
    {
        Cursors:: Cursors(const Cursor::Fund  &f,
                          const Species::Fund &s) noexcept :
        Cursor::List(f),
        speciesFund(s)
        {
        }

        Cursors:: ~Cursors() noexcept
        {
        }

        void Cursors:: operator()(const double c, const Actor &a)
        {
            Cursor::Node        *curr = new_node<const double,const Actor,const Species::Fund>(c,a,speciesFund);
            const Extended::Real xi   = (**curr).xi;
            try
            {
                switch(size)
                {
                    case 0:
                        push_back(curr);
                        return;
                        
                    case 1:
                        switch(Extended::Comp(xi,(**head).xi))
                        {
                            case negative: push_front(curr); return;
                            case positive: push_back(curr);  return;
                            case __zero__:
                                zombify(curr);
                                (**head) << *a;
                                break;
                        }
                        
                    default:
                        break;
                }
                throw exception("not implemented");
            }
            catch(...)
            {
                zombify(curr);
                throw;
            }

        }

    }



}


namespace yack
{
    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer() :
        speciesFund( new Species::Bank() ),
        xadd()
        {
        }


        
        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C0,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizing");

            for(const Equilibrium::Node *node=cl.standard.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                eq.display_compact(cl.pad(std::cerr << eq.name, eq) << " : ",C0,TopLevel) << std::endl;



            }

        }
        
    }

}

