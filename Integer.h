#include"DCDNode.h"
typedef unsigned long long ullong;
class Integer
{
public:
	DCDNode Vector,Buffer;
	DCDNode Pitch;
	bool Init;

	char* ArrayShowc()
	{
	   return  Vector.ArrayShowc();
	}
	
	void Add(unsigned int value)
	{
	  ullong Enum=Vector.GetLength()-1;
	  while(Enum>=0&&value!=0)
	  {
	  	Vector[Enum]+=value%10;
		value/=10;
	   	Enum--;
	  }
	  while(value!=0)
	  {
		Vector.AddTop(value%10+'0');
		value/=10;
	  }
	  Enum=Vector.GetLength()-1;
	  while(Enum>=1)
	  (Vector[Enum]>'9'?(Vector[Enum-1]+=1,Vector[Enum]-=10):0),Enum--;
	  Vector[0]>'9'?(Vector.AddTop('1'),Vector[1]-=10):0;     
	}
	void Add(Integer&value)
	{
	  ullong Enum=Vector.GetLength()-1;
	  ullong VEnum=value.Vector.GetLength()-1;
	 while(Enum>=0&&VEnum>=0)
	  {
	  	Vector[Enum]+=value.Vector[VEnum];
		
	   	Enum--;VEnum--;
	  }
	  while(VEnum>=0)
	  {
		Vector.AddTop(value.Vector[VEnum]);
		VEnum--;
	  }
	  Enum=Vector.GetLength()-1;
	  while(Enum>=1)
	  (Vector[Enum]>'9'?(Vector[Enum-1]+=1,Vector[Enum]-=10):0),Enum--;
	  Vector[0]>'9'?(Vector.AddTop('1'),Vector[1]-=10):0;     
	}
	void Times(Integer value)
	{
		long long int Enum=Vector.GetLength()-2-value.Vector.GetLength();
		long long int VEnum=0;
		Buffer=Vector;
		//Vector[Enum]='0'+
	}

	void Assign(unsigned int value)
	{
	   Vector.DelAll();
	   Vector[0]='0'+value%10;
	   value/=10;
	   while(value!=0)
	   Vector.AddTop('0'+value%10),
	   value/=10;
	}
	void operator =(unsigned int value)
	{
	   Assign(value);
	}
	void operator =(Integer value)
	{
	   //Vector.DelAll();
	   Vector=value.ArrayShowc();
	}
	void operator +=(unsigned int value)
	{
	   Add(value);
	}
	void operator +=(Integer value)
	{
	  Add(value);
	}
	void FreeOut()
	{
	   Vector.ClearArray();
	}
	Integer(unsigned int value)
	{
	   if(Init==false)return;
	   Vector.Settle();
	   Vector[0]='0'+value%10;
	   value/=10;
	   while(value!=0)
	   Vector.AddTop('0'+value%10),
	   value/=10;
	   Init=true;
	}
	Integer(unsigned long value)
	{
	   if(Init==false)return;
	   Vector.Settle();
	   Vector[0]='0'+value%10;
	   value/=10;
	   while(value!=0)
	   Vector.AddTop('0'+value%10),
	   value/=10;
	   Init=true;
	}
	Integer()
	{
	   if(Init==false)return;
	   Vector.Settle();
	   Vector[0]='0';
	   Init=true;
	}
	Integer(ullong value)
	{
	   if(Init==false)return;
	   Vector.Settle();
	   Vector[0]='0'+value%10;
	   value/=10;
	   while(value!=0)
	   Vector.AddTop('0'+value%10),
	   value/=10;
	   Init=true;
	}
};
