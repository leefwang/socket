#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 255
#define MAXPENDING 5


int main(int argc, char** argv)
{

int server_socket;

int client_socket;

char client_ip[20];


//argument의 숫자가 일치하지않을경우 오류출력후 종료
if(argc =! 2)
{
printf("Argument lack\n");
exit(-1);
}


//포트번호 5000

int server_port = 5000;
printf("%d \n",server_port);

struct sockaddr_in server_addr;

struct sockaddr_in client_addr;

int client_addrlen = INET_ADDRSTRLEN;



char buf[BUFSIZE];

int buf_len = 0;



server_socket = socket(AF_INET, SOCK_STREAM, 0);

if(server_socket == -1)

{

fprintf(stderr, "server : socket fail\n");

exit(-1);

}

memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
server_addr.sin_port = htons(server_port);

if(bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)

{
fprintf(stderr, "Server : bind fail\n");
exit(-1);
}

if(listen(server_socket, MAXPENDING) == -1)

{
fprintf(stderr, "Server : listen fail\n");
}


//무한루프 구현
while(1)
{
client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addrlen);
if(client_socket == -1)
{
fprintf(stderr, "Server : accept fail\n");
exit(-1);
}

inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, INET_ADDRSTRLEN);
printf("Server : %s client is connected.\n",client_ip);

buf_len = recv(client_socket, buf, BUFSIZE, 0);

if (buf_len > 0)

{
printf("Received message - ");
send(client_socket, buf , sizeof(buf), 0);
memset(buf,0,sizeof(buf));
}

//메세지 전송 후 클라이언트 소켓 닫기
close(client_socket);
printf("Server : %s client is disconnected.\n",client_ip);

}
//서버 소켓 닫기
close(server_socket);
return 0;
}