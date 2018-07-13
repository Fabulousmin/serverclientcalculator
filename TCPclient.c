#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>

//에러처리
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n; //본인 소켓, 포트번호, 크기
    struct sockaddr_in serv_addr; // 서버의 주소 구조체
    struct hostent *server;
    //값들을 저장할 버퍼
    char buffer[1024];
    //실행시 입력하는 요소들을 올바르게 입력하지 못했을 때 안내
    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]); //마지막으로 받는 값을 포트번호로 바꿔줌
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //클라이언트가 이용하는 소켓을 설정
    //소켓생성 실패시
    if (sockfd < 0) 
        error("ERROR opening socket");
    //입력한 서버의 ip값을 서버값으로 지정
    server = gethostbyname(argv[1]);
    //서버값 지정에 실패했을때
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //서버 구조체를 0으로 초기화
    //서버 구조체값 설정
    serv_addr.sin_family = AF_INET; 
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    //서버와 connect()를 진행, 이후 server에서 accept()이하 문이 실행
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
  
    //전달받는 값과 계산을 위한 변수들
    int num1 ;
    int num2 ;
    int ans;
    int choice;
    
    
    
  S:bzero(buffer,256);						
    n = read(sockfd,buffer,255);				//서버의 문자열 값을 읽음
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d" , &num1);						//첫번째 수를 입력
    write(sockfd, &num1, sizeof(int));     			//그 수를 서버에 전달
	
	
	
    bzero(buffer,256);						
    n = read(sockfd,buffer,255);				//서버의 문자열 값을 읽음
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d" , &num2);						//두번째 수를 입력
    write(sockfd, &num2, sizeof(int));     			//두번째 수를 서버에 전달
	
	
    	
    bzero(buffer,256);						
    n = read(sockfd,buffer,255);				//서버의 문자열 값을 읽음
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s",buffer);
    scanf("%d" , &choice);						//연산의 값 입력
    write(sockfd, &choice, sizeof(int));     			//선택한 연산값을 서버로 전달

	if (choice == 5)
	goto Q;

    read(sockfd , &ans , sizeof(int));				//서버로 부터 답을 읽어오고
    printf("Server - The answer is : %d\n" , ans);		//답을 얻는다
	
    if(choice != 5)
    	goto S;
    	
Q:  printf("통신 끝\n");
     	
       
   
    
    close(sockfd);
    return 0;
}

