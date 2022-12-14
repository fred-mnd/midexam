#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct food{
	char name[100];
	int price;
	int dish;
	food* next;
} *fHead=NULL, *fTail=NULL;

struct customer{
	char name[100];
	int hashing;
	food* oHead, *oTail;
	customer* next;
} *cHead[9999];

unsigned long hash(unsigned char *str){
        unsigned long hash = 5381;
        int c;
        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
}

food* createFood(char name[], int price, int dish){
	food* node = (food*)malloc(sizeof(food));
	strcpy(node->name, name);
	node->price=price;
	node->dish=dish;
	node->next=NULL;
	return node;
}

customer* createCustomer(char name[]){
	customer* node = (customer*)malloc(sizeof(customer));
	strcpy(node->name, name);
	node->oHead=NULL;
	node->oTail=NULL;
	node->next=NULL;
	return node;
}

void pushFood(char name[], int price, int dish){
	food* node=createFood(name, price, dish);
	if(!fHead) fHead=fTail=node;
	else{
		fTail->next=node;
		fTail=node;
	}
}

int orderFood(customer* cust, char name[], int dish){
	food* curr=fHead;
	while(curr&&strcmp(curr->name, name)!=0) curr=curr->next;
	if(!curr) return 1;
	if(dish<1) return 3;
	else if(dish>curr->dish) return 2;
	else{
		food* node=createFood(name, curr->price*dish, dish);
		if(!cust->oHead) cust->oHead = cust->oTail =node;
		else{
			cust->oTail->next=node;
			cust->oTail=node;
		}
	}
	curr->dish-=dish;
	return 0;
}

bool popFood(char name[]){
	if(!fHead) return false;
	else if(strcmp(fHead->name,name)==0){
		food* temp=fHead;
		fHead=temp->next;
		temp->next=NULL;
		free(temp);
	}
	else if(strcmp(fTail->name,name)==0){
		food* temp=fHead;
		while(temp->next!=fTail){
			temp=temp->next;
		}
		temp->next=NULL;
		free(fTail);
		fTail=temp;
	}
	else{
		food* curr=fHead;
		while(curr->next&&strcmp(curr->next->name,name)!=0){
			curr=curr->next;	
		}
		if(!curr->next) return false;
		food* temp=curr->next;
		curr->next=temp->next;
		temp->next=NULL;
		free(temp);
	}
	return true;
}

void insertCustomer(char name[]){
	int hashing = hash((unsigned char*)name)%9999;
	customer* node=createCustomer(name);
	while(cHead[hashing]) hashing++;
	cHead[hashing] = node;
	node->hashing=hashing;
}

bool printFood(){
	if(!fHead){
		puts("No dishes available");
		return false;
	}
	puts("\t\tBude's Menu");
	puts("===================");
	printf("%-3s  %-20s  %-10s  %-10s\n", "No.", "Name", "Quantity", "Price");
	food* curr=fHead;
	for(int i=0;curr;i++){
		printf("%-3d  %-20s  %-10d  %-10d\n", i+1, curr->name, curr->dish, curr->price);
		curr=curr->next;
	}
	return true;
}

bool capitalCheck(char augh[]){
	for(int i=0;i<strlen(augh);i++){
		if((augh[i]<'a'||augh[i]>'z')&&augh[i]!=' ') return false;
	}
	return true;
}

bool uniqueCheck(char augh[]){
	if(!fHead) return true;
	food* curr=fHead;
	while(curr){
		if(strcmp(curr->name,augh)==0) return false;
		else curr=curr->next;
	}
	return true;
}

customer* customerCheck(char query[]){
	int hashing=hash((unsigned char *)query)%9999;
	customer* curr=cHead[hashing];
	while(curr&&strcmp(curr->name, query)!=0){
		curr=curr->next;
	}
	if(!curr) return NULL;
	else return curr;
}


//----------------------------------------------------------------

void mainMenu();
void addDish();
void removeDish();
void addCustomer();
void searchCustomer();
void viewWarteg();
void order();
void payment();

int main(){
	mainMenu();
	
	return 0;
}

void mainMenu(){
	system("cls");
	puts("1. Add Dish");
	puts("2. Remove Dish");
	puts("3. Add Customer");
	puts("4. Search Customer");
	puts("5. View Warteg");
	puts("6. Order");
	puts("7. Payment");
	puts("8. Exit Warteg");
	printf(">> ");
	int mainMenuChoice;
	scanf("%d",&mainMenuChoice); getchar();
	if(mainMenuChoice<1||mainMenuChoice>8) mainMenu();
	else if(mainMenuChoice==1) addDish();
	else if(mainMenuChoice==2) removeDish();
	else if(mainMenuChoice==3) addCustomer();
	else if(mainMenuChoice==4) searchCustomer();
	else if(mainMenuChoice==5) viewWarteg();
	else if(mainMenuChoice==6) order();
	else if(mainMenuChoice==7) payment();
}

void addDish(){
	system("cls");
	char name[100];
	int price;
	int dish;
	do{
		printf("Insert the name of the dish [Lowercase letters]: ");
		scanf("%[^\n]",&name); getchar();
	} while(capitalCheck(name)==false||uniqueCheck(name)==false);
	do{
		printf("Insert the price of the dish [1000..50000]: ");
		scanf("%d",&price); getchar();
	} while(price<1000||price>50000);
	do{
		printf("Insert the quantity of the dish [1..999]: ");
		scanf("%d",&dish); getchar();
	} while(dish<1||dish>999);
	pushFood(name, price, dish);
	puts("The dish has been added!");
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}

void removeDish(){
	system("cls");
	if(printFood()==false){
		printf("Press enter to continue...");
		scanf("[^\n]"); getchar();
		mainMenu();
		return;
	}
	puts("================================================");
	char query[100];
	printf("Insert dish's name to be deleted: ");
	scanf("%[^\n]", query); getchar();
	if(popFood(query)==true){
		puts("The dish has been removed!");
	}
	else{
		puts("The dish is not available.");
	}
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}

void addCustomer(){
	system("cls");
	char name[100];
	do{
		printf("Insert the customer's name [Without space]: ");
		scanf("%[^\n]", name); getchar();
	} while(strchr(name, ' ')!=NULL);
	insertCustomer(name);
	puts("Customer has been added!");
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}

void searchCustomer(){
	system("cls");
	printf("Insert the customer's name to be searched: ");
	char query[100];
	scanf("%[^\n]", query); getchar();
	if(customerCheck(query)==false) printf("%s is not present\n", query);
	else printf("%s is present\n", query);
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}

void viewWarteg(){
	system("cls");
	puts("Customer's list");
	int count=1;
	for(int i=0;i<9999;i++){
		if(!cHead[i]) continue;
		customer* curr=cHead[i];
		while(curr){
			printf("%d. %s\n", curr->hashing, curr->name);
			count++;
			curr=curr->next;
		}
	}
	if(count==1) puts("There is no customer yet.");
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}
void order(){
	system("cls");
	char name[100];
	customer* node;
	do{
		printf("Insert the customer's name: ");
		scanf("%[^\n]", name); getchar();
		node=customerCheck(name);
		if(node==NULL) printf("%s is not present.\n", name);
	} while(node==NULL);
	int totalDish;
	printf("Insert the amount of dish: ");
	scanf("%d", &totalDish); getchar();
	for(int i=0;i<totalDish;i++){
		do{
			char dishName[100];
			int quantity;
			printf("[%d] Insert the dish's name: ", i+1);
			scanf("%[^\n]", dishName); getchar();
			printf("[%d] Insert the dish's quantity: ", i+1);
			scanf("%d", &quantity); getchar();
			printf("\n");
			int check=orderFood(node, dishName, quantity);
			if(check==0) break;
			else if(check==1) printf("The dish you order is not available.\n");
			else if(check==2) printf("The dish amount is not enough.\n");
			else if(check==3) printf("Please enter the proper amount of the dish.\n");
		} while(true);
	}
	puts("Order success!");
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	mainMenu();
}

void payment(){
	system("cls");
	int index;
	do{
		printf("Insert the customer's index: ");
		scanf("%d",&index); getchar();
		if(cHead[index]) break;
		puts("There is nocustomer with the index.");
	} while(true);
	customer* curr=cHead[index];
	int totalPrice=0;
	printf("%s\n", curr->name);
	food* currFood=curr->oHead;
	for(int i=1;currFood;i++, currFood=currFood->next){
		printf("[%d] %s x%d\n", i, currFood->name, currFood->dish);
		totalPrice+=currFood->price;
	}
	printf("Total: Rp%d\n", totalPrice);
	printf("Press enter to continue...");
	scanf("[^\n]"); getchar();
	free(curr);
	cHead[index]=NULL;
	mainMenu();
}