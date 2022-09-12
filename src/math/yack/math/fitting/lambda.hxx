
template <> lambda<real_t>:: ~lambda() throw() {}


template <> lambda<real_t>:: lambda() : field_type( clid(), layout1D(pmin(),pmax()), NULL)
{
    assert(lower<0);
    assert(upper>0);
    field_type &self = *this;
    coerce(self[0]) = 1;
    for(unit_t i=1;  i<=upper;++i) coerce(self[i]) = self[i-1] * 10;
    for(unit_t i=-1; i>lower;--i)  coerce(self[i]) = self[i+1] / 10;

}

template <> real_t  lambda<real_t>::  initialize(int &p) const throw()
{
    static const int  pini = pmin()/2;
    const field_type &self = *this;
    return self[p=pini];
}

template <> void lambda<real_t>:: decrease(int &p) const throw()
{
    static const int _ = pmin();
    if(--p<_) p=_;
}

template <> bool lambda<real_t>:: increase(int &p) const throw()
{
    static const int _ = pmax();
    if(p>=_)
    {
        return false;
    }
    else
    {
        ++p;
        return true;;
    }
}
