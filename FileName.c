#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Node {
	int id;
	char ver[50];
	struct Node* next;

};
struct Graph {
    int numVer;
	int addnumVer;
	struct Node** adj;

};
struct QueueNode {
	int data;
	struct QueueNode* next;
};
struct Queue {
	struct QueueNode* front;
	struct QueueNode* rear;
};
void show_menu(struct Graph* g) {
	printf("\n");
	printf("+=========================================+\n");
	printf("|          METRO MAP SYSTEM               |\n");
	printf("+=========================================+\n");
	printf("Stations: [%d]  \n", g->addnumVer);
	printf("\n");
	printf("1. Add Station\n");
	printf("2. Connect Two Stations\n");
	printf("3. Show Metro Map\n");
	printf("4. Find Shortest Path (BFS)\n");
	printf("5. Delete Station\n");
	printf("6. Export to File\n");
	printf("7. Import from File\n");
	printf("0. Exit\n");
	printf("\n");
	printf(">> ");
}
struct Graph* creat_metro(int numVer) {
	struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
	g->numVer = numVer;
	g->addnumVer = 0;
	g->adj = (struct Node**)malloc(sizeof(struct Node*)* numVer);
	for (int i = 0; i < g->numVer; i++) {
		g->adj[i] = NULL;
	}
	return g;

	
}
void create_stat(struct Graph* g) {
	if (g->addnumVer>=g->numVer) {
		printf("Overflow\n");
		return;

	}
	char name[50];
	printf("Enter name ");
	gets_s(name, 50);
	name[strcspn(name, "\n")] = '\0';
	struct Node* NewNode = (struct Node*)malloc(sizeof(struct Node));
	strcpy(NewNode->ver, name);
	NewNode->id = g->addnumVer;
	NewNode->next = NULL;
	g->adj[g->addnumVer] = NewNode;
	printf("Station added\n");
	g->addnumVer++;



}
struct Node* create_stat11(struct Graph* g,int id) {
	
	struct Node* NewNode = (struct Node*)malloc(sizeof(struct Node));
	strcpy(NewNode->ver,g->adj[id]->ver);
	NewNode->id =g->adj[id]->id;
	NewNode->next = NULL;
	return NewNode;


}
void add_edge(struct Graph* g) {
	if (g->addnumVer < 2) {
		printf("Need atleast 2 stationd\n");
		return;
	}
	for (int i = 0; i < g->addnumVer; i++) {
		printf("%d. %s\n", g->adj[i]->id, g->adj[i]->ver);
	}
	int id1, id2;
	printf("\nEnter first station ID: ");
	scanf_s("%d", &id1);
	printf("Enter second station ID: ");
	scanf_s("%d", &id2);
	getchar();  
	if (id1 < 0 || id1 >= g->addnumVer ||
		id2 < 0 || id2 >= g->addnumVer) {
		printf("Invalid station ID!\n");
		return;
	}
	if (id1 == id2) {
		printf("Cannot connect station to itself!\n");
		return;
	}
	struct Node* Node1 = create_stat11(g,id2);
	Node1->next = g->adj[id1]->next;
	g->adj[id1]->next = Node1;
	struct Node* Node2 = create_stat11(g, id1);  
	Node2->next = g->adj[id2]->next;             
	g->adj[id2]->next = Node2;

	printf("Connected: %s <-> %s\n", g->adj[id1]->ver, g->adj[id2]->ver);


}
void printMetroMap(struct Graph* g) {
	printf("\n========== METRO MAP ==========\n");

	if (g->addnumVer == 0) {
		printf("(No stations added yet)\n");
		printf("================================\n");
		return;
	}

	for (int i = 0; i < g->addnumVer; i++) {
		struct Node* station = g->adj[i];
		printf("Station %d (%s): ", station->id, station->ver);

		struct Node* temp = station->next;

		if (temp == NULL) {
			printf("(isolated)");
		}
		else {
			printf("<-> ");
			while (temp != NULL) {
				printf("%d (%s)", temp->id, temp->ver);
				if (temp->next != NULL) {
					printf(", ");
				}
				temp = temp->next;
			}
		}

		printf("\n");
	}

	printf("================================\n");
}
void export_file(struct Graph* g) {
	FILE* fp = fopen("metro_map.txt", "w");;
	if (fp == NULL) {
		printf("Error opening file\n");
		return;

	}
	fprintf(fp,"%d\n", g->addnumVer);
	for (int i = 0; i < g->addnumVer; i++) {
		fprintf(fp, "%s\n", g->adj[i]->ver);
	}
	for (int i = 0; i < g->addnumVer; i++) {
		struct Node* temp = g->adj[i]->next;
		while (temp != NULL) {
			int n = temp->id;
			if (i < n) {
				fprintf(fp, "%d,%d\n", i, temp->id);
			}
			temp = temp->next;


		}
	}
	fclose(fp);

}
void import_file1(struct Graph* g) {
	FILE* fp = fopen("metro_map.txt", "r");;
	if (fp == NULL) {
		printf("Error opening file\n");
		return;

	}
	g->addnumVer = 0;
	int numStat;
	fscanf(fp,"%d\n", &numStat);
	for (int i = 0; i < numStat; i++) {
		char line[50];
		fgets(line, 50, fp);
		line[strcspn(line, "\n")] = '\0';
		struct Node* New = (struct Node*)malloc(sizeof(struct Node));
		New->id = g->addnumVer;
		strcpy(New->ver, line);
		New->next = NULL;
		g->adj[g->addnumVer] =New;
		g->addnumVer++;


	}
	int id1, id2;
	while (fscanf(fp, "%d,%d\n", &id1, &id2) == 2) {
		struct Node* node1 = create_stat11(g, id2);
		node1->next = g->adj[id1]->next;
		g->adj[id1]->next = node1;
		struct Node* node2 = create_stat11(g, id1);
		node2->next = g->adj[id2]->next;
		g->adj[id2]->next = node2;
	}

	fclose(fp);
	printf("Imported from metro_map.txt (%d stations)\n", g->addnumVer);

}
struct Queue* create_q() {
	struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = NULL;
	q->rear = NULL;
	return q;

}
int isEmpty(struct Queue* q) {
	return (q->front == NULL);
}
void enque(struct Queue* q,int val) {
	struct QueueNode* NewNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
	NewNode->data = val;
	NewNode->next = NULL;
	if (q->front==NULL && q->rear == NULL) {
		q->front = NewNode;
		q->rear = NewNode;

	}
	else {
		q->rear->next = NewNode;
		q->rear = NewNode;


	}

}
int dequeue(struct Queue* q) {
	if (isEmpty(q)) {
		return -1;
	}
	struct QueueNode* temp= q->front;
	int res = temp->data;
	q->front = q->front->next;
	if (q->front == NULL) {
		q->rear = NULL;

	}
	free(temp);
	return res;

	

}
void find_path(struct Graph* g) {
	if (g->addnumVer < 2) {
		printf("Need atleast 2 stationd\n");
		return;
	}
	for (int i = 0; i < g->addnumVer; i++) {
		printf("%d. %s\n", g->adj[i]->id, g->adj[i]->ver);
	}
	int start, end;
	printf("\nEnter first station ID: ");
	scanf_s("%d", &start);
	printf("Enter second station ID: ");
	scanf_s("%d", &end);
	getchar();
	if (start < 0 || start >= g->addnumVer ||
		end < 0 || end>= g->addnumVer) {
		printf("Invalid station ID!\n");
		return;
	}
	if (start == end) {
		printf("Cannot connect station to itself!\n");
		return;
	}
	int* vis = (int*)malloc(sizeof(int)*g->addnumVer);
	for (int i = 0; i < g->addnumVer; i++) {
		vis[i] = 0;

	}
	int* par = (int*)malloc(sizeof(int) * g->addnumVer);
	for (int i = 0; i < g->addnumVer; i++) {
		par[i] = -1;
	}

	struct Queue* q = create_q();
	vis[g->adj[start]->id] = 1;
	enque(q, g->adj[start]->id);
	while (!isEmpty(q)) {
		int cur = dequeue(q);
		if (cur == end) {



			break;
		}
		struct Node* temp = g->adj[cur]->next;
		while (temp != NULL) {
			int n = temp->id;
			if (vis[n] == 0) {
				vis[n] = 1;
				par[n] = cur;
				enque(q, n);
				

			}

			temp = temp->next;
		}
		printf("\n");


	}
	int path[50];
	int pathLen = 0;
	int cur = end;
	while (cur != -1) {
		path[pathLen] = cur;
		pathLen++;
		cur = par[cur];

	}
	printf("\nShortest path(%d stations)\n", pathLen);
	for (int i = pathLen - 1; i >= 0;i--) {
		printf("%s", g->adj[path[i]]->ver);
		if (i > 0) {
			printf(" -> ");
		}
		
	}
	printf("\n");
	free(par);
	free(vis);



	
}
void del(struct Graph* g) {
	if (g->addnumVer == 0) {
		printf("No stations to delete!\n");
		return;
	}
	printf("\nAvailable stations:\n");
	for (int i = 0; i < g->addnumVer; i++) {
		printf("%d. %s\n", i, g->adj[i]->ver);
	}
	int statid;
	printf("\nEnter station ID to delete: ");
	scanf_s("%d", &statid);
	getchar();
	if (statid< 0 || statid >= g->addnumVer) {
		printf("Invalid ID!\n");
		return;
	}
	char stn[50];
	strcpy(stn, g->adj[statid]->ver);
	for (int i = 0; i < g->addnumVer; i++) {
		if (i == statid)continue;
		struct Node* cur= g->adj[i];
		struct Node* prew = NULL;
		while (cur != NULL) {
			if (cur->id == statid) {
				if (prew == NULL) {
					g->adj[i] = cur->next;

				}
				else {
					prew->next = cur->next;

				}
				struct Node* del = cur;
				cur = cur->next;
				free(del);

			}
			else {
				prew = cur;
				cur = cur->next;

			}
		}


	}
	struct Node* temp = g->adj[statid];
	while (temp != NULL) {
		struct Node* del = temp;
		temp = temp->next;
		free(del);


	}
	for (int i = statid; i < g->addnumVer-1; i++) {
		g->adj[i] = g->adj[i + 1];


	}
	for (int i = 0; i < g->addnumVer - 1; i++) {
		g->adj[i]->id = i;
		struct Node* edge = g->adj[i]->next;
		while (edge != NULL) {
			if (edge->id > statid) {
				edge->id--;
			}
			edge = edge->next;
		}
	}

	g->addnumVer--;

	printf("Station '%s' deleted!\n",stn);
}
int main() {
	struct Graph* g = creat_metro(50);
	int choice;

	while (1) {
		show_menu(g);
		scanf_s("%d", &choice);
		getchar(); 

		switch (choice) {
		case 1:
			create_stat(g);
			break;

		case 2:
			add_edge(g);
			break;

		case 3:
			printMetroMap(g);
			break;

		case 4:
			find_path(g);

			break;

		case 5:
			del(g);
			break;

		
		case 6:
			
			export_file(g);
			break;

		case 7:
			
			import_file1(g);
			

			break;

		case 0:
			printf("Exiting... Goodbye!\n");
			
			return 0;

		default:
			printf("Invalid choice!\n");
		}
	}

	return 0;
}