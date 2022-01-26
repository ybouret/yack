public:
//______________________________________________________________
//
//! fit session
//______________________________________________________________
inline bool fit(sample_type              &s,
                sequential_type          &func,
                writable<ORDINATE>       &aorg,
                const readable<bool>     &used,
                const readable<ORDINATE> &scal,
                control                  *ctrl)
{
    //----------------------------------------------------------
    //
    // initialize session
    //
    //----------------------------------------------------------
    self_type &self = *this;
    unit_t     p    = 0; // lambda selector
    size_t     r    = 1; // round indicator
    YACK_LSF_PRINTLN(clid << " <round#" << r << ">");

    //----------------------------------------------------------
    //
    // first round
    //
    //----------------------------------------------------------
    switch(round(s,p,func,aorg,used,scal,ctrl))
    {
        case round_process: break;
        case round_failure:
            YACK_LSF_PRINTLN(clid << " singular starting parameters");
            YACK_LSF_PRINTLN(clid << " <round#" << r << "/>");
            return false;

        case round_success:
            if(self.converged(aorg))
            {
                YACK_LSF_PRINTLN(clid << " converged starting parameters");
                YACK_LSF_PRINTLN(clid << " <round#" << r << "/>");
                return true;
            }
            break;
    }


    //----------------------------------------------------------
    //
    // other rounds
    //
    //----------------------------------------------------------
    ORDINATE D2old = D2;
NEXT_ROUND:
    ++r;
    YACK_LSF_PRINTLN(clid << " <round#" << r << ">");
    switch(round(s,p,func,aorg,used,scal,ctrl))
    {
            //--------------------------------------------------
            //
            // singularity
            //
            //--------------------------------------------------
        case round_failure:
            YACK_LSF_PRINTLN(clid << " singular parameters");
            YACK_LSF_PRINTLN(clid << " <round#" << r << "/>");
            return false;


            //--------------------------------------------------
            //
            // successful step
            //
            //--------------------------------------------------
        case round_success:
            //--------------------------------------------------
            // test convergence on variable
            //--------------------------------------------------
            if(self.converged(aorg))
            {
                YACK_LSF_PRINTLN(clid << " converged parameters, D2=" << D2);
                YACK_LSF_PRINTLN(clid << " <round#" << r << "/>");
                return true;
            }

            //--------------------------------------------------
            // test convergence on D2
            //--------------------------------------------------
            if(std::abs(D2old-D2) <= self.dtol * D2old )
            {
                YACK_LSF_PRINTLN(clid << " converged D2=" << D2);
                YACK_LSF_PRINTLN(clid << " <round#" << r << "/>");
                return true;
            }

            //--------------------------------------------------
            // update D2old
            //--------------------------------------------------
            D2old =  D2;
            goto NEXT_ROUND;

            //--------------------------------------------------
            //
            // not settled yet
            //
            //--------------------------------------------------
        case round_process:
            D2old = D2;
            goto NEXT_ROUND;
    }

    // never get here!
    return false;

}
