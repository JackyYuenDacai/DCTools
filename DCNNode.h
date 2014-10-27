#include<stdlib.h>
#include<string.h>
#ifndef DCDNODE_H

#define DCDNODE_H
#define abs(value) (value<0?-value:value)

typedef unsigned char byte ;
class DCDNStr
{
  	struct NodeLink
  	{
	  	unsigned char c;
		NodeLink*next,*unext;
	};
	NodeLink*head,*nail;
	NodeLink*pointer;
	struct TurnLog
	{
		void*Addr;
		TurnLog*next;
	};
	TurnLog*LogHead,*LogNow;
	long long pointNum;
	long long NodeCount;
public:
	void Alloc(){
		head=(NodeLink*)malloc(sizeof(NodeLink));
		nail=head;
		head->unext=NULL;
		NodeCount=0;
		pointer=nail;
		pointNum=-1;
		LogHead=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogHead;LogNow->Addr=0;
	}
	DCDNStr(long long len){
		Alloc();
		AddNode(len);
	}
	DCDNStr(){
		Alloc();
	}
	void AddNode(long long NCount){
		for(;NCount>0;NCount--){
			NodeCount++;
			nail->next=(NodeLink*)malloc(sizeof(NodeLink));
                        (nail->next)->unext=nail;
                        nail=nail->next;
		}
	}
	void AddArrayLog(void*Addr){
		LogNow->Addr=Addr;
		LogNow->next=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogNow->next;LogNow->Addr=0;
	}
	void ClearArrayLog(){
		TurnLog*Enum;
		LogNow=LogHead;
		if(LogHead->Addr==0)
			return;
		free(LogHead->Addr),LogNow=LogHead->next;
		while(LogNow->Addr!=0)
		{
			free(LogNow->Addr);
                        Enum=LogNow;
			LogNow=LogNow->next;
                        free(Enum);
		}
		free(LogNow);
		LogNow=LogHead,LogNow->Addr=0;
	}
	NodeLink* NodeSet(long long index){
	
		if(index<0)return NULL;
		if((index+1)>NodeCount)
       		return (AddNode((index+1)-NodeCount),nail);
       		if(abs(NodeCount/2-index)<abs(index-pointNum)){
       			NodeLink*retNode;
       			switch(int(index>=(NodeCount/2)))
			{
			case 0:
				retNode=head->next;
				for(long long i=0;i<index;i++)
				retNode=retNode->next;
				pointNum=index;
				pointer=retNode;
				return retNode;
		 	case 1:
				retNode=nail;
				for(long long i=0;i<(NodeCount-index-1);i++)
					retNode=retNode->unext;
				pointNum=index;
				pointer=retNode;
				return retNode;
			}
       		}
       		else{
       			switch(int(index>=pointNum)){
       			case 0:
       				for(long long i=0;i<=pointNum-index;i++)
       				pointer=pointer->unext,pointNum--;
       				return pointer;
       			case 1:
       				for(long long i=0;i<=index-pointNum;i++)
       				pointer=pointer->next,pointNum++;
       				return pointer;
       			}
       			
       		}
       		return NULL;
	}
	void DelNode(long long index){
		if(index>=NodeCount||index<0)
		return;
		if(index==NodeCount-1)
		{
			nail=(nail->unext);free(nail->next);
			nail->next=NULL;NodeCount--;
			return;
		}
		NodeLink*DelN=NodeSet(index);
		DelN->unext->next=DelN->next;
		DelN->next->unext=DelN->unext;
		free(DelN);
		NodeCount--;
	}
	void DelAll(){
		for(long long i=0;i<NodeCount;i++)
			nail=nail->unext,
			free(nail->next);
		NodeCount=0;
		head=nail;head->next=nail;nail->unext=head;
	}
	char*ToCharStr(){
		NodeLink*save=head->next;
        	char*retArray=(char*)malloc(size_t(sizeof(char)*(NodeCount+1)));
       		for(long long i=0;i<NodeCount;i++)
			retArray[i]=char(save->c),
			save=save->next;
		retArray[NodeCount]='\0';
		AddArrayLog(retArray);
		return retArray;
	}
	operator char*(){
		return ToCharStr();
	}
	operator const char*(){
		return (const char*)ToCharStr();
	}
	long long GetLength(){
		return NodeCount;
	}
	void AddString(char*Str,int length){
		NodeLink*save=nail;
		AddNode(length);save=save->next;
		for(long long i=0;i<length;i++)
		{
			save->c=Str[i];
			save=save->next;
		}
	}
	unsigned char& operator [](long long index){
		return (NodeSet(index)->c);
	}
	unsigned char& operator [](long index){
		return (NodeSet(index)->c);
	}	
	unsigned char& operator [](int index){
		return (NodeSet(index)->c);
	}
	unsigned char& operator [](short index){
		return (NodeSet(index)->c);
	}
	void operator =(char* Str){
		this->DelAll();
		this->AddString(Str,strlen(Str));
	}
	void operator =(const char* Str){
		this->DelAll();
		this->AddString((char*)Str,strlen((char*)Str));
	}
	void operator +=(char Lett){
		NodeSet(NodeCount+1)->c=Lett;
	}
	void operator +=(char* Str){
		AddString(Str,strlen(Str));
	}
	void operator +=(const char* Str){
		AddString((char*)Str,strlen((char*)Str));
	}
	void operator=(DCDNStr&dcnode){
		DelAll();
		AddString(dcnode.ToCharStr(),dcnode.GetLength());
	}
	
	~DCDNStr(){
		DelAll();
		ClearArrayLog();
		free(LogHead);
		free(head);
	}
};
template <class t>
class DCDNode{

};

#endif
