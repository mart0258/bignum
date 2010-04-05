#include <string>

template <int S>
class bignum
{
private:

public:
	unsigned short num[S];
	bignum()
	{
		for (int i=0; i<S; ++i)
		{
			num[i]=0;
		}
	}

	bignum(short j)
	{
		num[0]=j;
		for (int i=1; i<S; ++i)
		{
			num[i]=0;
		}
	}

	bignum(int j)
	{
		num[0]=j;
		if (S>1) num[1]=j>>16;
		for (int i=2; i<S; ++i)
		{
			num[i]=0;
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

	/* Arithmetic operators */
	bignum operator + (const bignum &rhs) const 
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
		return ret; 
	}

	bignum operator - (const bignum &rhs) const 
	{
		bignum ret;
		int carry=0;
		int v;
		for (int i=0; i<S; ++i)
		{
			v=num[i]-rhs.num[i]-carry;
			carry=0;
			if (rhs.num[i]>num[i])
				carry=1;
			ret.num[i]=v;
		}
		return ret; 
	}

	bignum operator * (const bignum &rhs) const 
	{
		bignum ret;
		int carry=0;
		unsigned int v;

		for (int i=0; i<S; ++i)
		{
			carry=0;
			for (int j=0; i+j<S; ++j)
			{
				v=num[i]*rhs.num[j]+carry;
				carry=(v+ret.num[i+j])>>16; // 16 bits
				ret.num[i+j]+=v;
			}
		}
		return ret;
	}

	bignum operator / (const bignum &rhs) const 
	{
		bignum div = rhs;

		int p=1;

		while (!(div.num[S-1]) && (div < *this))
		{
			div <<=16;
			p+=16;
			if (p==S*16+1) throw -1;
		}
		while (!(div.num[S-1]&0x8000) && (div < *this))
		{
			div <<=1;
			p++;
			if (p==S*16+1) throw -1;
		}

		bignum ret;
		bignum cur(*this);

		if (div<cur)
		{
			cur-=div; 
			ret.num[0]|=1;
		}
	
		while (--p>=0)
		{
			ret <<=1;
			if (div<=cur)
			{
				cur-=div; 
				ret.num[0]|=1;
			}
			div>>=1;
		}

		return ret; 
	}

	bignum operator % (const bignum &rhs) const 
	{
		bignum div = rhs;

		int p=1;

		while (!(div.num[S-1]) && (div < *this))
		{
			div <<=16;
			p+=16;
			if (p==S*16+1) throw -1;
		}
		while (!(div.num[S-1]&0x8000) && (div < *this))
		{
			div <<=1;
			p++;
			if (p==S*16+1) throw -1;
		}

		bignum ret;
		bignum cur(*this);

		if (div<cur)
		{
			cur-=div; 
			ret.num[0]|=1;
		}
	
		while (--p>=0)
		{
			ret <<=1;
			if (div<=cur)
			{
				cur-=div; 
				ret.num[0]|=1;
			}
			div>>=1;
		}

		return cur; 
	}

	bignum operator << (int rhs) const
	{
		bignum ret; 
		int i=0, j=0; // 16 bits 

		while (i<S)
		{
			if (i<rhs/16)
			{
				ret.num[i++]=0;
			}
			else
			{
				if (j<S)
				{
					ret.num[i]=num[j];
				} else
				{
					ret.num[i]=0;
				}
				++i, ++j;
			}
		}
		rhs%=16; 
		
		if (rhs>0)
		{
			for (i=S-1; i>=0; --i)
			{
				ret.num[i]<<=rhs;
				if (i>0)
					ret.num[i]|=ret.num[i-1]>>(16-rhs);
			}
		}

		return ret; 
	}

	bignum operator >> (int rhs) const
	{
		bignum ret; 
		int i=0, j=rhs/16; // 16 bits 

		while (i<S)
		{
			if (j<S)
			{
				ret.num[i]=num[j];
			} else
			{
				ret.num[i]=0;
			}
			++i, ++j;
		}
		rhs%=16; 
		
		if (rhs>0)
		{
			for (i=0; i<S; ++i)
			{
				ret.num[i]>>=rhs;
				if (i<S-1)
					ret.num[i]|=ret.num[i+1]<<(16-rhs);
			}
		}

		return ret; 
	}

	/* Comparison operators */
	bool operator < (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]>rhs.num[i])
				return false;
			if (num[i]<rhs.num[i])
				return true;
		}
		if (num[0]==rhs.num[0]) return false;
		return true;
	}

	bool operator > (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]<rhs.num[i])
				return false;
			if (num[i]>rhs.num[i])
				return true;
		}
		if (num[0]==rhs.num[0]) return false;
		return true;
	}

	bool operator <= (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]>rhs.num[i])
				return false;
			if (num[i]<rhs.num[i])
				return true;
		}
		return true;
	}

	bool operator >= (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]<rhs.num[i])
				return false;
			if (num[i]>rhs.num[i])
				return true;
		}
		return true;
	}

	
	bool operator == (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]!=rhs.num[i])
				return false;
		}
		return true;
	}

	bool operator != (const bignum &rhs) const
	{
		for (int i=S-1; i>=0; i--)
		{
			if (num[i]==rhs.num[i])
				return true;
		}
		return false;
	}

	/* Other functions */
	std::string toString()
	{
		bignum ten;
		ten.num[0]=10;
		bignum pl;
		pl.num[0]=1;
		std::string ret;
		bignum cur=*this;
		bignum div;

		int ex=0;
		int z=0;

		do
		{
			bignum test = cur / pl;
			if (test<ten) break;
			pl = pl * ten;
			++ex;
		}while (1);

		while (ex-->=0)
		{
			div=cur/pl;

			z=1;
			ret+=('0'+div.num[0]);
			cur-=div*pl;
			pl=pl/ten;
		}

		if (ret.size()==0) ret+='0';

		return ret;
	}

	std::string toStringSigned()
	{
		bignum ten;
		ten.num[0]=10;
		bignum pl;
		pl.num[0]=1;
		std::string ret;
		bignum cur=*this;
		bignum div;

		int ex=0;
		int z=0;

		if (cur.num[S-1]&0x8000) {
			ret +='-';
			cur=~cur;
		}

		do
		{
			bignum test = cur / pl;
			if (test<ten) break;
			pl = pl * ten;
			++ex;
		}while (1);

		while (ex-->=0)
		{
			div=cur/pl;

			z=1;
			ret+=('0'+div.num[0]);
			cur-=div*pl;
			pl=pl/ten;
		}

		if (ret.size()==0) ret+='0';

		return ret;
	}

	bignum operator ~()
	{
		bignum ret;
		for (int i=0; i<S; ++i)
		{
			ret.num[i]=~num[i];
		}
		return ret;
	}

	/* Arithmetic/assignment operators */
	bignum &operator += (const bignum &rhs) 
	{
		return *this = *this + rhs;
	}

	bignum &operator -= (const bignum &rhs) 
	{
		return *this = *this - rhs;
	}

	bignum &operator *= (const bignum &rhs) 
	{
		return *this = *this * rhs;
	}

	bignum &operator /= (const bignum &rhs) 
	{
		return *this = *this / rhs;
	}

	bignum &operator %= (const bignum &rhs) 
	{
		return *this = *this % rhs;
	}

	bignum &operator <<= (int rhs) 
	{
		return *this = *this << rhs;
	}
	bignum &operator >>= (int rhs) 
	{
		return *this = *this >> rhs;
	}
};

