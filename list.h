struct clientSocketForDisplay {
    SOCKET socket;
    struct clientSocketForDisplay *pre;
    struct clientSocketForDisplay *next;

};

struct clientSocketForDisplay * head = NULL;
struct clientSocketForDisplay * tail = NULL;

struct clientSocketForDisplay * createNewNode (SOCKET socket) {
    struct clientSocketForDisplay *newNode = (struct clientSocketForDisplay *)malloc(sizeof(struct clientSocketForDisplay));
    newNode->socket = socket;
    newNode->pre = newNode->next = NULL;
    return newNode;
}


void showSockets(void) {
    struct clientSocketForDisplay * node = head;
    if(node==NULL) {
        return;
    }
    printf("��ǰ����Ϊ:");
    do {
        printf("%d->",node->socket);
        node = node->next;
    } while(node!=NULL);
    printf("\n");
}
void addToDisplayList(SOCKET socket) {
    printf("���socket %d\n",socket);
    if(head==NULL) {
        head = tail = createNewNode(socket);
    } else {
        struct clientSocketForDisplay *newNode =  createNewNode(socket);
        tail->next = newNode;
        newNode->pre = tail;
        tail = newNode;
    }
    showSockets();
}


void removeFromDisplayList(struct clientSocketForDisplay *client) {
    if(client==NULL) {
        printf("NULL�ڵ�\n");
        return;
    }
    printf("׼���Ƴ��ڵ�%d\n", client->socket);
    if(client->next !=NULL && client->pre!=NULL) {
        printf("�Ƴ���ͨ�ڵ�%d\n", client->socket);
        client->pre->next = client->next;
        client->next->pre = client->pre;
    } else if(client->next ==NULL && client->pre==NULL) {
        printf("����ֻ��1���ڵ�%d\n", client->socket);
        head = tail = NULL;
    } else if(client->next==NULL) {
        printf("�Ƴ�tail�ڵ�%d\n", client->socket);
        client->pre->next = NULL;
        tail = client->pre;
    } else {
        printf("�Ƴ�head�ڵ�%d\n", client->socket);
        head = client->next;
        client->next->pre = NULL;
    }
    free(client);
    showSockets();
}

