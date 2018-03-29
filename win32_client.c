#include<stdbool.h>
#include<stdio.h>
#include<winsock2.h>
#include"common.h"
#include"common_client.h"
#include<windows.h>
#include<time.h>


_Bool readFromStdin(char buffer[], int size) {
    char *ptr = fgets(buffer, size, stdin);
    if(ptr==NULL) {
        puts("��ȡ����\n");
        return false;
    }
    size_t oriLength = strlen(buffer);
    if(oriLength<1) {
        printf("��ֵĳ���%u\n", oriLength);
        return false;
    }
    buffer[oriLength-1] = 0;//ɾ���س�
    return true;

}

void chat(SOCKET clientSocket) {
    time_t start,end;
    time(&start);
    char bufferForSpeak[CLIENT_BUFFER_SIZE] = {0};
    int length = 0;
    int resultOfSend = 0;
    while(true) {
        _Bool result = readFromStdin(bufferForSpeak, CLIENT_BUFFER_SIZE);
        if(!result) {
            puts("��ȡ����ʧ�ܣ�������\n");
            continue;
        }
        length = strlen(bufferForSpeak);
        if(length>CLIENT_BUFFER_SIZE/2) {
            puts("����̫��������Ʒ��Գ���\n");
            fflush(stdin);
            continue;
        }
        time(&end);
        //printf("%u\n",end-start);
        if(end-start<1) {
            printf("���ϻ�˵��̫����-_-%u\n", end-start);
            continue;
        }
        time(&start);
        //printf("���͵�������%s\n", bufferForSpeak);
        resultOfSend = send(clientSocket, bufferForSpeak, length, 0);
        if(resultOfSend<0) {
            printf("����ʧ��,��������%d��������\n", resultOfSend);
            continue;
        }
    }
}
#define BUFFER_SIZE 30

void setNickName(SOCKET clientSocket) {
    char nickname[BUFFER_SIZE] = {0};
    char * nicknamePtr = 0;
    while(true) {
        puts("�����������������е��ǳƣ�������10Ӣ����ĸ����\n");
        _Bool result = readFromStdin(nickname, BUFFER_SIZE);
        if(!result) {
            continue;
        }
        int lengthOfNickname = strlen(nickname);
        if(lengthOfNickname==0) {
            puts("�ǳƲ���Ϊ��\n");
            continue;
        }
        if(lengthOfNickname>MAX_NICKNAME_LENGTH) {
            printf("�ǳ�̫���� %d\n", lengthOfNickname);
             fflush(stdin);
            continue;
        }
        int resultOfSend = send(clientSocket, nickname, lengthOfNickname, 0);
        printf("�����ǳƵĽ��Ϊ%d�����Կ�ʼ����\n", resultOfSend);
        return;
    }
}
int main(int argc, char *argv[]) {
    init();
    SOCKET clientSocket = createSocket();
    struct sockaddr_in clientAddress = createClientAddressWithPort(6666);
    struct sockaddr_in serverAddress = getServerAddress();
    bindSocketWithAddress(clientSocket, clientAddress, sizeof(clientAddress));
    connectToServer(clientSocket, serverAddress);
    setNickName(clientSocket);
    chat(clientSocket);
    return 0;
}
