
#include <stdio.h>
#include <string>
template <int S, int D>
class bigfixed
{
private:

public:
	union
	{
		unsigned short num[S+D];
		struct 
		{
			unsigned short whnum[S];
			unsigned short denum[D];
		} spl;
	}dat;
	unsigned short &num[](dat.spl.whnum);
	bignum()
	{
		for (int i=0; i<S+D; ++i)
		{
			num[i]=0;
		}
	}

	bignum(short j)
	{
		for (int i=0; i<S+D; ++i)
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


	bignum operator + (const bignum &rhs) const 
	{
		bignum ret;
		int carry=0;
		int v;
		for (int i=S+D-1; i>=0; --i)
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
		for (int i=S+D-1; i>=0; --i)
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
		int t; 

		for (int i=0; i<S+D; ++i)
		{
			carry=0;
			for (int j=S-i; i+j<S; ++j)
			{
				t=i+j-S;
				if (t<0 || t>=S+D) continue;
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
		//TODO
	}

	bignum operator % (const bignum &rhs) const 
	{
		bignum div = rhs;

		int p=1;

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
		//TODO
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

	bool  operator > (const bignum &rhs) const
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

	bool  operator <= (const bignum &rhs) const
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

		//printf ("ToString() called\n");
		do
		{
			bignum test = cur / pl;
			//printf ("%d-", test.num[0]);
			if (test<ten) break;
			pl = pl * ten;
			++ex;
			//printf ("%d\n", ex);
		}while (1);

		while (ex-->=0)
		{
			div=cur/pl;
			/*if (div.num[0]==0 && !z)
			{
				pl=pl/ten;
				continue;
			}*/

			z=1;
			//printf ("Appending: %d\n", div.num[0]);
			ret+=('0'+div.num[0]);
			cur-=div*pl;
			pl=pl/ten;
		}

		if (ret.size()==0) ret+='0';

		return ret;
	}

	bignum &operator += (const bignum &rhs) 
	{
		*this = *this + rhs;

		return *this;
	}

	bignum &operator -= (const bignum &rhs) 
	{
		*this = *this - rhs;

		return *this;
	}

	bignum &operator *= (const bignum &rhs) 
	{
		*this = *this * rhs;

		return *this;
	}

	bignum &operator /= (const bignum &rhs) 
	{
		*this = *this / rhs;

		return *this;
	}

	bignum &operator <<= (int rhs) 
	{
		*this = *this << rhs;

		return *this;
	}
	bignum &operator >>= (int rhs) 
	{
		*this = *this >> rhs;

		return *this;
	}




};

