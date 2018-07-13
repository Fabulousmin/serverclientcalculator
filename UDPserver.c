#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[3];
char output[2];

//서버와 클라이언트의 주소 구조체
struct sockaddr_in serv_addr;
struct sockaddr_in clnt_addr;

int rec, result;
int port; // 포트번호
int sk; // 소켓
int op1, op2, outcome; //피연산자 1, 피연산자 2, 결과값

int main (int argc, char  *argv[]) {
   //서버 실행시 적절한 요건을 갖추지 못했을 경우 안내
    if (argc < 2) {
        fprintf(stderr,"Error no port provided EG: %s [port].\n",argv[0]);
        exit(1);
    } 
   //요건을 갖췄을 경우 
   else {
        port = atoi(argv[1]);
    }
    //소켓 설정 및 서버 주소 구조체 설정 
    sk = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);
    
    //바인드
    result = bind(sk, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result < 0) {
        printf("Error in Bind %d\n", port);
        exit(1);
    }
    //바인드 성공시 client의 응답을 대기
    else{
        printf("Waiting using portnumber %d.....\n", port);
    }

	//클라이언트 주소 구조체 초기화
	bzero(&clnt_addr,sizeof(clnt_addr));
        int tam = sizeof(clnt_addr);
	//클라이언트가 입력했던 input배열을 받아옴
        recvfrom(sk, input, sizeof(input), 0, (struct sockaddr*)&clnt_addr, (unsigned*)&tam);
	//input배열에서 원하는 값을 추출 (피연산자1 과 2)
        op1 = input[1] - '0';
        op2 = input[2] - '0';
        //연산자값으로 사칙연산 조건문 실행
	switch(input[0]) {
            case '+':
                outcome = op1+op2;
                break;
            case '-':
                outcome = op1-op2;
                break;
            case '/':
                outcome = op1/op2;
                break;
            case '*':
                outcome= op1*op2;
                break;
        }
	//10의 값보다 클경우
        output[0] = outcome/10 + '0';
        output[1] = outcome%10 + '0';
        //결과를 output배열에 담아서 전송
	result = sendto(sk,output,strlen(output),0,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr));
    	
	close(sk);

}
