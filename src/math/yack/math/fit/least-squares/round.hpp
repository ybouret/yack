private:
//______________________________________________________________
//
//! perform one round
/**
 \param s    a sample
 \param p    current power indicator
 \param func a sequential fit function
 \param aorg current parameters
 \param used currently used parameters
 \param scal scaling factor for gradient of func
 \param ctrl control function
 */
//______________________________________________________________
inline round_result round(sample_type              &s,
                          unit_t                   &p,
                          sequential_type          &func,
                          writable<ORDINATE>       &aorg,
                          const readable<bool>     &used,
                          const readable<ORDINATE> &scal,
                          control                  *ctrl)
{
    assert(aorg.size()==used.size());
    assert(aorg.size()==scal.size());

    //----------------------------------------------------------
    //
    //
    // initialize memory, set shrinking to false
    // and check power
    //
    //
    //----------------------------------------------------------
    least_squares<ABSCISSA,ORDINATE> &self = *this;          // alias
    const size_t                      npar = aorg.size();    // parameters
    const variables                  &vars = *s;             // variables of sample
    self.initialize(npar);
    p = clamp(self.pmin,p,self.pmax);

    //----------------------------------------------------------
    //
    // create the objective function to minimize
    //
    //----------------------------------------------------------
    call1D  f1d = { atry, aorg, step, s, func };
    typename real_func:: template call<call1D> g(f1d);


    //----------------------------------------------------------
    //
    //
    // compute all metrics @aorg: may be costly
    //
    //
    //----------------------------------------------------------
    D2 = s.D2_full(func,aorg,used,drvs,scal); // set D2
    const ORDINATE D2ini = D2;                // starting point
    s.finalize();                             // make curvature symmetrical


    YACK_LSF_PRINTLN(clid << " [p=" << p << "] => lambda=" << lam[p]);
    if(verbose) vars.display(std::cerr,aorg,"\taorg.");
    YACK_LSF_PRINTLN("D2ini = " << D2ini);


    //----------------------------------------------------------
    //
    //
    // making the best out of full metrics
    //
    //
    //----------------------------------------------------------

COMPUTE_CURVATURE:
    //----------------------------------------------------------
    //
    // try compute curvature
    //
    //----------------------------------------------------------
    if(!self.compute_curvature(p,s.curv,vars,used))
    {
        //------------------------------------------------------
        // singular
        //------------------------------------------------------
        YACK_LSF_PRINTLN(clid << " singular parameters");
        return round_failure;
    }


    //----------------------------------------------------------
    //
    // from estimated curvature and gradient, compute step
    //
    //----------------------------------------------------------
    tao::v1::load(step,s.beta);
    algo->solve(curv,step);


    if(verbose) vars.display(std::cerr,step,"\tstep.");

    //----------------------------------------------------------
    //
    // compute FULL trial parameters: atry=aorg+step
    //
    //----------------------------------------------------------
    tao::v1::add(atry,aorg,step);

    //----------------------------------------------------------
    //
    // check post-processing
    //
    //----------------------------------------------------------
    if(ctrl)
    {

        if( !(*ctrl)(atry) )
        {
            // something happened
            YACK_LSF_PRINTLN(clid << " post-processing warning");
            if(!self.shrink(p))
            {
                YACK_LSF_PRINTLN(clid << " post-processing caused failure");
                return round_failure;
            }
        }
    }

    //----------------------------------------------------------
    //
    // finalize trial position/step
    //
    //----------------------------------------------------------
    tao::v1::sub(step,atry,aorg);       // recompute step in any case
    ORDINATE D2try = s.D2(func,atry);   // compute trial value
    YACK_LSF_PRINTLN("D2try = " << D2try);
    if(verbose) vars.display(std::cerr,atry,"\tatry.");

    //----------------------------------------------------------
    //
    // check if we moved
    //
    //----------------------------------------------------------
    if(!self.has_moved()) {
        YACK_LSF_PRINTLN(clid << " did not move!");
        return round_success;
    }

    //----------------------------------------------------------
    //
    // study result
    //
    //----------------------------------------------------------
    if(D2try<D2ini)
    {
        YACK_LSF_PRINTLN(clid << " forwarding...");

        //------------------------------------------------------
        // acceptable step
        //------------------------------------------------------
        if(!shrinking)
        {
            // local adjustment
            D2try = self.optimize(g,D2ini,D2try);
            exit(0);
        }

        //------------------------------------------------------
        // decreased
        //------------------------------------------------------
        tao::v1::set(aorg,atry);
        D2 = D2try;
        if(shrinking)
        {
            // needed shrinking, not stabilized
            YACK_LSF_PRINTLN(clid << " round process");
            return round_process;
        }
        else
        {
            // without shrinking, stabilized
            YACK_LSF_PRINTLN(clid << " round success");
            p = max_of(--p,self.pmin);
            return round_success;
        }

    }
    else
    {
        //------------------------------------------------------
        // need to go slower
        //------------------------------------------------------
        YACK_LSF_PRINTLN(clid << " backtracking...");
        if(!self.shrink(p))
        {
            // cannot shrink more => failure
            YACK_LSF_PRINTLN(clid << " round failure");
            return round_failure;
        }
        goto COMPUTE_CURVATURE;
    }

}
