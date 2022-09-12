
template <> lambda<real_t>:: ~lambda() throw() {}


template <> lambda<real_t>:: lambda() : field_type( clid(), layout1D(pmin(),pmax()), NULL)
{
    assert(lower<0);
    assert(upper>0);
    field_type &self = *this;
    coerce(self[0]) = 1;
    for(unit_t i=1;  i<=upper;++i) coerce(self[i]) = self[i-1] * 10;
    for(unit_t i=-1; i>=lower;--i) coerce(self[i]) = self[i+1] / 10;

}

