#include<winsock2.h>
#include<stdio.h>
#include<stdbool.h>
#include"common.h"
#include<pthread.h>
#include"list.h"
#define MAX_LENGTH_OF_NICKNAME 100
pthread_mutex_t lock ;
struct sockaddr_in createServerAddress(int port) {
    /**
        ����һ��internet��address��address����3���֣�
        1��Э���
        2���˿�
        3���������ӽ�����ip
        addrin��sockaddr_in��oo�����о��Ǹ����֮��Ĺ�ϵ
    **/
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );
    address.sin_addr.s_addr = INADDR_ANY;
    return address;
}

void linstenTo(SOCKET socket, int lengthOfQueue) {
    //��socketתΪ����״̬��Ҳ���Ǽ�������������
    listen(socket, lengthOfQueue);
}

_Bool isClientForDisplay(struct sockaddr_in * clientAddress) {
    return ntohs(clientAddress->sin_port) == 7777;
}


struct sockAndAddr {
    SOCKET clientSocket;
    struct sockaddr_in clientAddress;
    struct sockAndAddr *ptr;
};

struct threadArgs {
    struct sockAndAddr socketAndAddress;
    char nickname[MAX_LENGTH_OF_NICKNAME];
};

void sendToClientsForDisplay(char *message, int length, struct sockAndAddr *socketAndAddress) {
    if(head==NULL) {
        printf("û�пͻ�����Ҫ����\n" );
        return;
    }
    //�ڷ��͵�����ǰ�����ip��ð��
    //printf("���͵��ͷ��˵�������========%s \n", message);

    struct clientSocketForDisplay * node = head;
    while(node!=NULL) {

        int  result = send(node->socket, message, length + 16 + 1, 0);
        printf("���͵�socket %d�Ľ����%d\n",node->socket,result);
        //ֻ��Ҫͬ������Ĳ�������
        pthread_mutex_lock(&lock);
        if(result==-1) {
            removeFromDisplayList(node);
        }
        node = node->next;
        pthread_mutex_unlock(&lock);
    }
    free(message);
};

void getCompletedMessage(char *dest, char * ip, char * nickname, char *message){
  strcpy(dest, ip);
  strcat(dest, nickname);
  strcat(dest, ":");
  strcat(dest, message);
  printf("�û������շ�����ϢΪ%s\n", dest);
}
void getLeaveMessage(char *dest, char *ip, char *nickname){
    strcpy(dest, "========ϵͳ��Ϣ��ipΪ");
    strcat(dest, ip);
     strcat(dest, "���û�\"");
    strcat(dest, nickname);
    strcat(dest, "\"�Ͽ�����=======");
}

void *chat(void * param) {
    struct threadArgs *arg = (struct threadArgs *)param;
    printf("�û����ǳ��ǣ�%s", arg->nickname);

    struct sockAndAddr *socketAndAddress  = &(arg->socketAndAddress);
    SOCKET clientSocket = (*socketAndAddress).clientSocket;
    struct sockaddr_in clientAddress = socketAndAddress->clientAddress;
    char buffer[SERVER_BUFFER_SIZE] = {0};//�����û����룬��Ҫ�㹻��
    char message[SERVER_BUFFER_SIZE]={0};//��װ�����յ��ַ���

    int numberOfRead=0;
    char *ip = inet_ntoa(clientAddress.sin_addr);
    do {
        numberOfRead  = recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("��ȡ�������ݳ�����%d\n", numberOfRead);
        if(numberOfRead>0) {
            buffer[numberOfRead] = 0;//�޸�ĩβ�����ݲ�û��̫��Ӱ�죿
            getCompletedMessage(message, ip, arg->nickname, buffer);
            printf("ip %s�ύ������-----------��:%s\n",ip, message);
            sendToClientsForDisplay(message, strlen(message), socketAndAddress);
        }
    } while(numberOfRead>0);
    char leaveMessage[100] = {0};
    getLeaveMessage(leaveMessage, ip, arg->nickname);
    sendToClientsForDisplay(leaveMessage, strlen(leaveMessage), socketAndAddress);
    free(socketAndAddress);
    printf("client %s %d�ر�����\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port) );
    return 0;
}
void getNickname(SOCKET clientSocket, char buffer[], int lengthOfBuffer){
     int numberOfRead  = recv(clientSocket, buffer, lengthOfBuffer, 0);
     buffer[numberOfRead] = 0;
}

void createNewThreadToChat(SOCKET clientSocket, struct sockaddr_in clientAddress) {
    struct threadArgs * args =  (struct threadArgs *)malloc(sizeof(struct threadArgs));
    getNickname(clientSocket, args->nickname, MAX_LENGTH_OF_NICKNAME);
    printf("�ͻ����ύ���ǳ�Ϊ %s\n", args->nickname);
    args->socketAndAddress.clientSocket = clientSocket;
    args->socketAndAddress.clientAddress = clientAddress;
    pthread_t thread;
    /**
        ע�������о޴�Ŀӣ���java֮��ĸ߼����Բ�ͬ��������stack�Ϸ�����ڴ棬�ں����˳���������Ч
        ���socketAndAddress���߳�������Ч�ģ���˱�����heap�Ϸ����ڴ�

    **/
    int result = pthread_create(&thread, NULL, chat, args);
    //printf("�Ự�̴߳���״̬Ϊ%d\n", result);
}

void handleIncomingRequest(SOCKET serverSocket) {

    int sizeofAddress = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddress;//ÿ���½����������ж�Ӧ��socket��address
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &sizeofAddress);
    if(clientSocket==INVALID_SOCKET) {
        printf("�Ƿ��Ŀͻ������ӣ���������\n",WSAGetLastError());
        return;
    }
    printf("�յ��ͻ������ӣ�ip��%s %u %d\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port),clientSocket);
    if(isClientForDisplay(&clientAddress)) { //�ֱ���չʾ�ͻ���������
        printf("���%s:%d���б�", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
        addToDisplayList(clientSocket);
        return;
    } else {
        createNewThreadToChat(clientSocket, clientAddress);
    }
}





int main(void) {
    init();
    SOCKET serverSocket = createSocket();
    struct sockaddr_in serverAddress = createServerAddress(PORT_OF_SERVER);
    bindSocketWithAddress(serverSocket, serverAddress, sizeof(serverAddress));
    linstenTo(serverSocket, SOMAXCONN);
    pthread_mutex_init(&lock, NULL);
    while(true) {
        handleIncomingRequest(serverSocket);
    }
    return 0;
}
