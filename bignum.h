
template <int S>
class bignum
{
private:
	unsigned short num[S];

public:
	bignum()
	{
		for (int i=0; i<S; ++i)
		{
			num[S]=0;
		}
	}

	template <int X>
	operator bignum<X> () const
	{
		bignum<X> ret;
		for (int i=0; i<S && i<X; ++i)
		{
			ret.num[i]=i<S?num[i]:0;
		}
		return ret;
	}

	bignum operator + (bignum &rhs)
	{
		bignum ret;
		int carry=0;
		int v;
		for (int i=0; i<S; ++i)
		{
			v=num[i]+rhs.num[i]+carry;
			carry=((int)num[i]+(int)rhs.num[i])>>16; // 16 bits
			ret.num[i]=v;
		}
	}

	bignum operator - (bignum &rhs)
	{
		bignum ret;
		int carry=0;
		int v;
		for (int i=0; i<S; ++i)
		{
			v=num[i]-rhs.num[i]-carry;
			if (rhs.num[i]>num[i])
				carry=1;
			ret.num[i]=v;
		}
	}

	bignum operator * (bignum &rhs)
	{
		bignum ret;
		int carry=0;
		unsigned int v;
		for (int i=0; i<S; ++i)
		{
			ret.num[i]=0;
		}

		for (int i=0; i<S; ++i)
		{
			carry=0;
			for (int j=0; i+j<S; ++j)
			{
				v=num[i]*num[j]+carry;
				carry=(v+ret[i+j])>>16; // 16 bits
				ret[i+j]+=v;
			}
		}
		return ret;
	}


};

