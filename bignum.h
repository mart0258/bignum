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

   	bignum(char *str)
	{
		*this = str;
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
			carry=v>>16; // 16 bits
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
			if (v<0)
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

	bignum divideSigned (const bignum &rhs) const
	{
		bool sign;
		bignum left = *this;
		if (left.num[S-1]&0x8000)
		{
			sign^=true;
			left=~left +1;
		}
		bignum right = rhs;
		if (right.num[S-1]&0x8000)
		{
			sign^=true;
			right=~right + 1;
		}
		bignum result = left/right;

		if (sign)
			result=~result + 1;
		return result;
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

	bignum & operator ++()
	{
		for (int i=0; i<S; ++i)
			if (++num[i]!=0)
				break;
		return *this;
	}
	bignum & operator --()
	{
		for (int i=0; i<S; ++i)
			if (num[i]--!=0)
				break;
		return *this;
	}

	/* Functions */
	bignum pow (bignum exp)
	{
		bignum base = *this;
		bignum ret = 1;

		while (!exp.isZero())
		{
			if (exp.num[0]&1)
			{
				ret *= base;
			}
			base *= base; 
			exp >>= 1;
		}

		return ret;
	}

	bignum expmod (bignum exp, const bignum &mod)
	{
		bignum base = *this % mod;
		bignum ret;

		exp %= mod;

		while (!exp.isZero())
		{
			if (exp.num[0]&1)
			{
				ret = ret * base % mod;
			}
			base = base * base % mod; 
			exp >>= 1;
		}

		ret %= mod;
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
			cur=~cur+1;
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

	std::string toStringHex(char sep='\0')
	{
		std::string ret;
		
		for (int i=S-1; i>=0; --i)
		{
			for (int j=12; j>=0; j-=4)
			{
				int val = (num[i]>>j)&0xf;
				if (val<10) ret+='0'+val;
				else 
					ret+='A'-10+val;
			}
			if (sep && i) ret+=sep;
		}
		return ret;
	}

	bignum operator = (const char *str)
	{
		int i;
		for (i=0; i<S; ++i)
		{
			num[i]=0;
		}
		bignum ten(10);
		i=0;
		bool sign=false;
		if (str[i]=='-')
			sign=true, ++i;
		for (; str[i]; ++i)
		{
			
			if (str[i]<'0' || str[i]>'9')
				break;
			*this *= ten;
			*this+=(int)(str[i]-'0');
		}
		if (sign)
		{
			*this = ~*this+1;
		}

		return *this;
	}

	bignum operator ~() const
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

	/* Status functions */
	bool isZero()
	{
		for (int i=0; i<S; ++i)
			if (num[i]) return false;
		return true;
	}

	bool bit(int i)
	{
		return (num[i/16]>>(i%16))&1;
	}
};

