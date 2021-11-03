#include "yack/apex/integer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace apex
    {

        integer integer:: operator+() const
        {
            return *this;
        }

        integer & integer:: operator++()
        {
            static const int_type one = 1;
            const handle L(*this);
            const handle R(one);
            integer      I = add(L,R);
            xch(I);
            return *this;
        }

        integer integer:: operator++(int)
        {
            static const int_type one = 1;
            const handle L(*this);
            const handle R(one);
            integer      I = add(L,R);
            xch(I);
            return I;
        }


        integer integer:: add(const handle &lh, const handle &rh)
        {

            switch( __sign::pair(lh.s,rh.s) )
            {
                    //----------------------------------------------------------
                    //
                    // LHS < 0
                    //
                    //----------------------------------------------------------
                case nn_pair: // RHS < 0
                {
                    const  natural n = natural::add(lh,rh);
                    return integer(negative,n);
                }

                case nz_pair: // RHS == 0
                {
                    const  natural n(lh.entry,lh.words);
                    return integer(negative,n);
                }

                case np_pair: // RHS > 0
                {
                    switch( natural::scmp(lh,rh) )
                    {
                        case negative: /* |LHS| < |RHS| */
                        {
                            const  natural n = natural::sub(rh,lh);
                            return integer(positive,n);
                        }
                        case __zero__: /* |LHS| == |RHS| */ return integer();

                        case positive: /* |LHS| > |RHS|  */
                        {
                            const  natural n = natural::sub(lh,rh);
                            return integer(negative,n);
                        }
                    }
                    goto CORRUPTED;
                }

                    //----------------------------------------------------------
                    //
                    // LHS == 0
                    //
                    //----------------------------------------------------------
                case zn_pair: // RHS < 0
                {
                    const  natural n(rh.entry,rh.words);
                    return integer(negative,n);
                }
                case zz_pair: // RHS == 0
                    return integer();

                case zp_pair: // RHS > 0
                {
                    const  natural n(rh.entry,rh.words);
                    return integer(positive,n);
                }

                    //----------------------------------------------------------
                    //
                    // LHS > 0
                    //
                    //----------------------------------------------------------
                case pn_pair: // RHS < 0
                {
                    switch( natural::scmp(lh,rh) )
                    {
                        case negative: /* |LHS| < |RHS| */
                        {
                            const  natural n = natural::sub(rh,lh);
                            return integer(negative,n);
                        }
                        case __zero__: /* |LHS| == |RHS| */ return integer();

                        case positive: /* |LHS| > |RHS| */
                        {
                            const  natural n = natural::sub(lh,rh);
                            return integer(positive,n);
                        }
                    }
                    goto CORRUPTED;
                }

                case pz_pair: // RHS == 0
                {
                    const  natural n(lh.entry,lh.words);
                    return integer(positive,n);
                }

                case pp_pair: // RHS > 0
                {
                    const  natural n = natural::add(lh,rh);
                    return integer(positive,n);
                }
            }

        CORRUPTED:
            throw exception("integer::add(CORRUPTED)");
        }

    }

}
