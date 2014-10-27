#pragma once
#include"DCNNode.h"
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>

#define CLIENT_PORT 0xDCDD
#define MAX_CLIENTS 0xDC

class DCNSock{
public:
	bool IsS;
	bool Isipv4;
	bool IsError;
	bool IsBlocking;
	int ServerSock;
	int flags;
	int LRecvCount;
	char*RecvBuf;
	long ClientsCount;
	short Port;
	fd_set ClientCheck;
	fd_set ServerCheck;
	timeval CheckTimeout;
	DCNStr Addr;
	DCNStr LRecv;
	struct Client{
		short id;
		int sock_fd;
		sockaddr addr;
		socklen_t addrlen;
	};
	DCNode<Client> Clients;
	sockaddr_in ServAddr;
	struct{
		DCNStr ErrorStr;
		int ErrorNo;
		short Stage;
	}ErrorInfo;

	DCNSock(){
		Alloc();
	}
	DCNSock(bool IsS){
		this->IsS=IsS;
		this->Isipv4=true;
		Alloc();
	}
	bool SetAddress(char*Addr){
		char**ptr;
		char   str[32];
		struct hostent *hptr;
		if((inet_pton(AF_INET,Addr,&ServAddr.sin_addr))>0){
			this->Addr=Addr;
			return true;
		}
		else{
			 if((hptr = gethostbyname(Addr)) == NULL){
				GetErrorInfo(0);
        			return false;
        		}
        		this->Addr=(char*)inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
        		return true;
		}
		return false;
	}
	void SetIsServer(bool IsS){
		this->IsS=IsS;
	}
	void SetPort(short pn){
		this->Port=pn;
	}
	bool SetNonBlock(){
		flags=fcntl(ServerSock,F_GETFL,0);
		if(flags<0){
			GetErrorInfo(1);
    			return false;      
		}
		fcntl(ServerSock,F_SETFL,flags|O_NONBLOCK);
		return true;
	}
	void ClientSetFd(){
		FD_ZERO(&ClientCheck);
		FD_SET(ServerSock,&ClientCheck);
		
	}
	void ServerSetFd(){
		FD_ZERO(&ServerCheck);
		FD_SET(ServerSock,&ServerCheck);
		for(int i=0;i<Clients.GetLength();i++){
			FD_SET(Clients[i].sock_fd,&ServerCheck);
		}
	}
	DCNode<Client>& GetClients(){
		return Clients;
	}
	char* Recv(long Len,Client* Dest=NULL,short id=0){
		free(RecvBuf);
		RecvBuf=(char*)malloc(sizeof(char)*Len);
		int ret;
		if(this->IsS==false){
			
			if((ret=recv(ServerSock,RecvBuf,Len,0))<0){
				GetErrorInfo();
				return NULL;
			}
			else{
				LRecvCount=ret;
				LRecv.DelAll();
				LRecv.AddString(RecvBuf,ret);
				return RecvBuf;
			}
		}
		else{
			if(Dest==NULL){
				Dest=&Clients[id];
			}
			if((ret=recv(Dest->sock_fd,RecvBuf,Len,0))<0){
				GetErrorInfo();
				return NULL;
			}
			else{
				LRecvCount=ret;
				LRecv.DelAll();
				LRecv.AddString(RecvBuf,ret);
				return RecvBuf;
			}
		}
	}
	bool Send(char*data,long len=0,Client* Dest=NULL,short id=0){
		if(len==0)len=strlen(data)+1;
		if(IsS==false){
			if((send(ServerSock,(const char*)data,len,0))<0){
				GetErrorInfo();
				return false;
			}
			else{
				return true;
			}
		}
		else{
			
			if((send(Dest==NULL?Clients[id].sock_fd:Dest->sock_fd,(const char*)data,len,0))<0){
				GetErrorInfo();
				return false;
			}
			else{
				return true;
			}
		}
	}
	void CloseClient(short id){
		close(Clients[id].sock_fd);
		Clients.DelNode(id);
		ClientsCount--;
	}
	void CloseClients(){
		for(int i=0;i<Clients.GetLength();i++)
			close(Clients[i].sock_fd);
	}
	void CloseServer(){
		close(ServerSock);
	}
   	DCNode<short> 
   	ServerCheckRead(){
   		ServerSetFd();
		int ret=select(Clients.GetLength()+1,&ServerCheck,NULL,NULL,&CheckTimeout);
		
		if(ret<0){
			GetErrorInfo(3);
			return -1;
		}
		else if(ret==0){
			return ret;
		}
		DCNode<short> AbleList;
		return AbleList;
		for(short i=0;i<Clients.GetLength();i++){
			if(FD_ISSET(Clients[i].sock_fd,&ServerCheck))
			AbleList+=i;
		}
		return AbleList;
	}
   	bool ServerCheckAccept(){
   		FD_ZERO(&ServerCheck);
		FD_SET(ServerSock,&ServerCheck);
		int ret=select(Clients.GetLength()+1,&ServerCheck,NULL,NULL,&CheckTimeout);
		if(ret<0){
			GetErrorInfo(3);
			return false;
		}
		else if(ret==0){
			return false;
		}
   		if(FD_ISSET(ServerSock,&ServerCheck)){
   			return true;
   		}
   		return false;
   	}
   	bool ServerAccept(){
   		int i=Clients.GetLength();
		Client&Dest=Clients[i];
   		Dest.sock_fd=accept(ServerSock,(struct sockaddr*)(&(Dest.addr)),(&(Dest.addrlen)));
   		if(Dest.sock_fd<=0){
   			GetErrorInfo(3);
   			return false;
   		}
   		ClientsCount++;
   		return true;
   	}
	
	int ClientCheckRead(){
		ClientSetFd();
		int ret=select(ServerSock+1,&ClientCheck,NULL,NULL,&CheckTimeout);
		if(ret==-1){
			GetErrorInfo(3);
			return -1;
		}
		else if(ret==0){
			return ret;
		}
		if(FD_ISSET(ServerSock,&ClientCheck)){
			return 1;
		}
		return 2;
	}
	int ClientCheckWrite(){
		ClientSetFd();
		int ret=select(ServerSock+1,NULL,&ClientCheck,NULL,&CheckTimeout);
		if(ret==-1){
			GetErrorInfo(3);
			return -1;
		}
		else if(ret==0){
			return ret;
		}
		if(FD_ISSET(ServerSock,&ClientCheck)){
			return 1;
		}
		return 2;
	}
	void SetTimeout(long Sec,long USec=0){
		CheckTimeout.tv_sec=Sec;
		CheckTimeout.tv_usec=USec;
	}
	bool ServerInit(){
		if((ServerSock=socket(AF_INET,SOCK_STREAM,0))==-1){
			GetErrorInfo(1);
			return false;		
		}
		ServAddr.sin_family=AF_INET;
		ServAddr.sin_port=htons(Port);
		ServAddr.sin_addr.s_addr=htonl(INADDR_ANY);
		return true;
	}
	bool ServerBind(){
		if(bind(ServerSock,(struct sockaddr*)&ServAddr,sizeof(ServAddr))==-1){  
			GetErrorInfo(2);	
			return false;		
		}
		return true;
	}
	bool ServerListen(int num=0){
		if(num==0)num=MAX_CLIENTS;
		if((listen(ServerSock,num))==-1){
			GetErrorInfo(2);
			return false;
		}
		return true;
	}
	bool ClientInit(){
		if((ServerSock=socket(AF_INET,SOCK_STREAM,0))<0){
			GetErrorInfo(1);
			return false;		
		}
		ServAddr.sin_family=AF_INET;
		ServAddr.sin_port=htons(Port);
		if((inet_pton(AF_INET,(char*)Addr,&ServAddr.sin_addr))<=0){
			GetErrorInfo(1);	
			return false;
		}
		return true;
	}
	bool ClientConn(){
		if((connect(ServerSock,(struct sockaddr*)&ServAddr,sizeof(struct sockaddr)))!=0){ 
			GetErrorInfo(2);	
			return false;
		} 
		return true;
	}
	void Alloc(){
		Port=CLIENT_PORT;
		IsError=false;
		ClientsCount=0;
		ServerSock=0;
		memset(&ServAddr,0,sizeof(ServAddr));
		CheckTimeout.tv_sec=0;
		CheckTimeout.tv_usec=0;
		LRecvCount=0;
		RecvBuf=(char*)malloc(sizeof(char)*1);
	}
	void GetErrorInfo(short Stage=0){
		ErrorInfo.ErrorStr=strerror(errno);	
		ErrorInfo.ErrorNo=errno;
		ErrorInfo.Stage=Stage;
		IsError=true;
	}
	~DCNSock(){
		IsS==true?CloseClients():CloseServer();
	}

};
