#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

//Error 메시지코드
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	
     int sockfd, newsockfd, portno; //Sockfd(클라이언트 소켓) newsockfd(서버 소켓) portno(포트번호)
     socklen_t clilen;
     char buffer[1024]; //버퍼
     struct sockaddr_in serv_addr, cli_addr;//서버측 주소 구조체 및 클라이언트 측 주소 구조체 
     int n; 

     //서버 실행시 포트를 입력하지 않았을 때 사용법을 알려줌
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     //서버에서 이용할 소켓을 정의
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     //서버 소켓이 생성되지 않았을 경우 예외처리
     if (sockfd < 0) 
        error("ERROR opening socket");
  
     bzero((char *) &serv_addr, sizeof(serv_addr)); //서버 구조체를 0으로 초기화
     portno = atoi(argv[1]); //서버 실행시 받은 번호를 포트넘버로 부여 
     serv_addr.sin_family = AF_INET; //서버 구조체에서 형식
     serv_addr.sin_addr.s_addr = INADDR_ANY; //IP주소에 제약을 주지 않음
     serv_addr.sin_port = htons(portno); //포트넘버를 이용하여 주소를 부여
    
     //Bind
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) //바인드를 실패했을 때 오류처리 
              error("ERROR on binding");
    
     //Listen
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     //클라이언트에서 오는 메시지를 받을 소켓 생성
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     //소켓 생성에 실패할 시 예외처리
     if (newsockfd < 0) 
          error("ERROR on accept");
       
       
     //피연산자 1,2 와 결과값 그리고 연산자 선택  
     int num1 , num2 , ans , choice;
 S:  n = write(newsockfd,"Enter Number 1 : ",strlen("Enter Number 1")); //첫번째 번호를 요청     
     if (n < 0) error("ERROR writing to socket");
     read(newsockfd, &num1, sizeof(int));					     //첫번째 번호를 읽어옴
     printf("Client - Number 1 is : %d\n" , num1);
          
          
     n = write(newsockfd,"Enter Number 2 : ",strlen("Enter Number 2"));              //두번째 번호를 요청    
     if (n < 0) error("ERROR writing to socket");
     read(newsockfd, &num2, sizeof(int));					     //두번째 번호를 읽어옴
     printf("Client - Number 2 is : %d\n" , num2);
     
     
     n = write(newsockfd,"Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n",strlen("Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));         //Ask for choice    
     if (n < 0) error("ERROR writing to socket");
     read(newsockfd, &choice, sizeof(int));					     //연산자 선택한 값을 불러옴
     printf("Client - Choice is : %d\n" , choice);	
     
     //연산자 선택
      switch(choice)
     {
     	case 1:
     		ans = num1 + num2;
     		break;
     	case 2:
     		ans = num1 -num2;
     		break;
     	case 3:
     		ans = num1*num2;
     		break;
     	case 4:
     		ans = num1/num2;
     		break;
     	case 5 :
 		goto Q;
     		break;
     }
     //클라이언트에 값을 전달
     write(newsockfd , &ans , sizeof(int));
     if(choice != 5)
	goto S;   
    //통신의 종료
   Q:  close(newsockfd);
     close(sockfd);
     return 0; 
}
