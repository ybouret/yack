#include "yack/chem/aftermath.hpp"

namespace yack
{
    namespace chemical
    {
        aftermath:: aftermath() throw() : state( components::are_blocked ), value(0) {}
        
        aftermath:: ~aftermath() throw() {}
        
        aftermath:: aftermath(const aftermath &other) throw() :
        state(other.state),
        value(other.value)
        {
        }
        
        aftermath:: aftermath(const components::state s,
                              const double            v) throw() :
        state(s),
        value(v)
        {
            assert( !(components::are_blocked==state && fabs(value)>0) );
        }
        
        aftermath & aftermath:: operator=(const aftermath &other) throw()
        {
            coerce(state) = other.state;
            coerce(value) = other.value;
            return *this;
        }
    }
    
}

#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/triplet.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        
        
        enum search_extent
        {
            search_positive_extent,
            search_negative_extent
        };
        
        std::ostream & operator<<(std::ostream &os, const aftermath &am)
        {
            os << components::state_text(am.state) << " " <<std::setw(15) << am.value;
            return os;
        }
        
        //
        static inline
        double ridder_denom(const triplet<double> &f,
                            rmulops               &xmul)
        {
            //std::cerr << "ridder_denom(" << f << ")" << " xmul@" << &xmul << std::endl;
            const double absb = fabs(f.b);   assert(absb>0);
            xmul.resume(3);                  assert(0==xmul.size());
            xmul.ld(f.a);                    assert(1==xmul.size());
            xmul.ld(f.c);                    assert(2==xmul.size());
            xmul.upower(1.0/absb,2);         assert(4==xmul.size());
            const double rhs = xmul.query(); assert(rhs<=0);
            return absb * sqrt(1.0-rhs);
        }
        
        aftermath aftermath:: guess(const components       &comp,
                                    const double            K_eq,
                                    const readable<double> &Cini,
                                    writable<double>       &Cend,
                                    raddops                &xadd,
                                    rmulops                &xmul)
        {
            static const char fn[] = "chemical::aftermath::guess";
            assert(K_eq>0);assert(comp.size()>0);
            //const void *ADDR = &xmul;
            
            //------------------------------------------------------------------
            //
            // initialize Cend=Cini
            //
            //------------------------------------------------------------------
            iota::load(Cend,Cini);
            
            //------------------------------------------------------------------
            //
            // check if components are running
            //
            //------------------------------------------------------------------
            switch( comp.state_at(Cini) )
            {
                case components::are_blocked: return aftermath(); // early return
                case components::are_running: break;
            }
            
            //------------------------------------------------------------------
            //
            // create function from Cend
            //
            //------------------------------------------------------------------
            double                       AX = -1; // absolute Xi, must decrease at each turn
            components::mass_action_call FF = { K_eq, comp, Cend,xmul };
            xlimits_io                   io;
            
            //------------------------------------------------------------------
            //
            // outer loop for a given Cend
            //
            //------------------------------------------------------------------
        FIND_XI:
            {
                triplet<double>  x  = { 0, 0,    0 };           // initialize x.b=0
                triplet<double>  f  = { 0, FF(0), 0 };          // initialize f.b @Cend
                const sign_type  s  = __sign::of(f.b);          // check initial sign
                search_extent    d  = search_positive_extent;   // to be set w.r.t sign
                switch(s)
                {
                    case __zero__:  goto  SUCCESS;                    // early return using Cend
                    case positive: d = search_positive_extent; break; // xi>0
                    case negative: d = search_negative_extent; break; // xi<0
                }
                
                
                //--------------------------------------------------------------
                //
                // bracket solution
                //
                //--------------------------------------------------------------
                {
                    const xlimits &       xlms = comp.genuine_limits(io,Cend,0);
                    const xlimit  * const rlim = xlms.reac;
                    const xlimit  * const plim = xlms.prod;
                    
                    
                    switch(xlms.type)
                    {
                            
                        case limited_by_none: throw imported::exception(fn,"unexpected unlimited");
                            
                        case limited_by_both:
                            switch(d)
                            {
                                case search_positive_extent:
                                    x.a = x.b;      assert(fabs(x.a)<=0);
                                    f.a = f.b;      assert(f.a>0);
                                    x.c = rlim->xi; assert(x.c>0);
                                    f.c = FF(x.c);  assert(f.c<0);
                                    break;
                                    
                                case search_negative_extent:
                                    x.c = x.b; assert(fabs(x.c)<=0);
                                    f.c = f.b; assert(f.c<0);
                                    x.a = -(plim->xi);
                                    f.a = FF(x.a); assert(f.a>0);
                                    break;
                            }
                            break;
                            
                        case limited_by_reac:
                            assert(comp.prod.molecularity==0);
                            switch (d)
                            {
                                case search_negative_extent:
                                    x.c = x.b; assert(fabs(x.c)<=0);
                                    f.c = f.b; assert(f.c<0);
                                    x.a = -pow(K_eq,-1.0/comp.reac.molecularity);
                                    while( (f.a = FF(x.a)) <= 0 ) x.a += x.a;
                                    assert(x.a<0);
                                    assert(f.a>0);
                                    break;
                                    
                                case search_positive_extent:
                                    x.a = x.b;      assert(fabs(x.a)<=0);
                                    f.a = f.b;      assert(f.a>0);
                                    x.c = rlim->xi; assert(x.c>0);
                                    f.c = -1;
                                    break;
                            }
                            
                            break;
                            
                        case limited_by_prod:
                            assert(comp.reac.molecularity==0);
                            switch (d)
                            {
                                case search_negative_extent:
                                    x.c = x.b;          assert(fabs(x.c)<=0);
                                    f.c = f.b;          assert(f.c<0);
                                    x.a = -(plim->xi);  assert(x.a<0);
                                    f.a = K_eq;         assert(f.a>0);
                                    break;
                                    
                                case search_positive_extent:
                                    x.a = x.b; assert( fabs(x.a) <= 0);
                                    f.a = f.b; assert(f.a>0);
                                    x.c = pow(K_eq,1.0/comp.prod.molecularity);
                                    while( (f.c = FF(x.c)) >= 0 ) x.c += x.c;
                                    assert(x.c>0);
                                    assert(f.c<0);
                                    break;
                            }
                            break;
                            
                    }
                }
                
                //--------------------------------------------------------------
                //
                // we have a bracketed solution
                //
                //--------------------------------------------------------------
                assert(x.a<x.c);
                assert( __sign::product_of(f.a,f.c) == negative );
                
                //--------------------------------------------------------------
                //
                // find best Cend + nu * xi
                //
                //--------------------------------------------------------------
                {
                    double width = fabs(x.c-x.a); assert(width>0);
                ZFIND:
                    //----------------------------------------------------------
                    //
                    // take middle point
                    //
                    //----------------------------------------------------------
                    assert(f.a>0);
                    assert(f.c<0);
                    f.b = FF( x.b = clamp(x.a, 0.5*(x.a+x.c), x.c) ); assert(x.is_increasing());
                    
                    switch( __sign::of(f.b) )
                    {
                            //------------------------------------------------------
                            //
                            // early return
                            //
                            //------------------------------------------------------
                        case __zero__:
                            comp.move(Cend,x.b);
                            goto SUCCESS;
                            
                            //------------------------------------------------------
                            //
                            // hybrid step with f.b < 0
                            //
                            //------------------------------------------------------
                        case negative: {
                            //const double den = sqrt( squared(f.b) - f.a*f.c );
                            const double den = ridder_denom(f,xmul);
                            if(fabs(f.b)<den)
                            {
                                //--------------------------------------------------
                                // Ridder's step in [x.a:x.b]
                                //--------------------------------------------------
                                const double x_r = x.b+0.5*( width * (f.b/den) );
                                const double x_u = clamp(x.a,x_r, x.b );
                                const double f_u = FF(x_u);
                                
                                switch( __sign::of(f_u) )
                                {
                                    case __zero__:
                                        comp.move(Cend,x.b=x_u);
                                        goto SUCCESS;
                                        
                                    case negative:
                                        f.c = f.b = f_u;
                                        x.c = x.b = x_u;
                                        break;
                                        
                                    case positive: assert(f.b<0);
                                        f.c = f.b;
                                        x.c = x.b;
                                        f.a = (f.b = f_u);
                                        x.a = (x.b = x_u);
                                        break;
                                }
                                
                            }
                            else
                            {
                                //--------------------------------------------------
                                // fallback to bisection
                                //--------------------------------------------------
                                f.c = f.b;
                                x.c = x.b;
                            }
                        } break;
                            
                            //------------------------------------------------------
                            //
                            // hybrid step with f.b > 0
                            //
                            //------------------------------------------------------
                        case positive: {
                            //const double den = sqrt( squared(f.b) - f.a*f.c );
                            const double den = ridder_denom(f,xmul);
                            if(fabs(f.b)<den)
                            {
                                //--------------------------------------------------
                                // Ridder's step in [x.b:x.c]
                                //--------------------------------------------------
                                const double x_r = x.b+0.5*( width * (f.b/den) );
                                const double x_u = clamp(x.b,x_r, x.c );
                                const double f_u = FF(x_u);
                                
                                switch( __sign::of(f_u) )
                                {
                                    case __zero__:
                                        comp.move(Cend,x.b=x_u);
                                        goto SUCCESS;
                                        
                                    case negative: assert(f.b>0);
                                        f.a = f.b;
                                        x.a = x.b;
                                        f.c = (f.b = f_u);
                                        x.c = (x.b = x_u);
                                        break;
                                        
                                    case positive:
                                        f.a = f.b = f_u;
                                        x.a = x.b = x_u;
                                        break;
                                }
                            }
                            else
                            {
                                //--------------------------------------------------
                                // fallback to bisection
                                //--------------------------------------------------
                                f.a = f.b;
                                x.a = x.b;
                            }
                        } break;
                    }
                    
                    //--------------------------------------------------------------
                    // check reduction
                    //--------------------------------------------------------------
                    const double new_width = fabs(x.c-x.a);
                    if(new_width>=width)
                    {
                        //----------------------------------------------------------
                        // end of current reduction
                        //----------------------------------------------------------
                        const double xx = x.b;
                        const double ax = fabs(xx);
                        comp.move(Cend,xx);
                        if( AX>0 && (ax>=AX) )
                            goto SUCCESS;
                        
                        //----------------------------------------------------------
                        // prepare for next bracketing/solving
                        //----------------------------------------------------------
                        AX = ax;
                        goto FIND_XI;
                    }
                    
                    //--------------------------------------------------------------
                    // still reducing
                    //--------------------------------------------------------------
                    width = new_width;
                    goto ZFIND;
                }
            }
            
            
        SUCCESS:
            return aftermath(components::are_running,comp.estimate_extent(Cini,Cend,xadd));
        }
        
    }
    
}
