#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct sockaddr_in serv_addr;//서버주소 구조체

char input[3];//매개변수로 받은 값을 서버로 넘길때 담아둘 배열
char output[2];//서버에서 넘어온 값을 담아둘 배열

int port; // 포트번호
int sk; // 소켓
int rec; //recvfrom()의 결과값

int main (int argc, char *argv[]) {
   	//서버 호출시 적절한 요건을 갖추지 못했을 경우 안내
	 if (argc < 6) {
        fprintf(stderr,"usage %s [ip] [port] [operand] [op1] [op2]. EG: 127.0.0.1 7777 + 5 2.\n",argv[0]);
        exit(1);
 	   }
 	//갖췄을 경우
	else {
        port = atoi(argv[2]); //받은 값에서 포트번호를 추출
        printf("해당 포트번호로 연결 :  %d\n", port);
        input[0] =*argv[3]; //연산자를 추출하여 넘겨줄 배열에 담아둠
        input[1] =*argv[4]; //피연산자1을 추출하여 넘겨줄 배열에 담아둠
        input[2] =*argv[5]; //피연산자2를 추출하여 넘겨줄 배열에 담아둠
    }
    //소켓생성 및 보내줄 서버 주소 구조체의 세부 값을 할당 
    sk=socket (AF_INET, SOCK_DGRAM,0);
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr= inet_addr(argv[1]);
    serv_addr.sin_port= htons(port);
    //담아둔 배열을 서버로 넘겨줌
    sendto(sk,input,strlen(input),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    //서버 구조체의 크기
    int tam = sizeof(serv_addr);
    //서버로 부터 계산된 결과값을 output에 담아서 받음
    rec = recvfrom(sk,output,sizeof(output), 0, (struct sockaddr*)&serv_addr, (unsigned*) &tam);
    //배열 초기화
    output[rec] = '\0';
    //결과값 출력
    printf("result: %d\n",output[rec]-'0');

    close(sk);
}
