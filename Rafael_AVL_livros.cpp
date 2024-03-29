#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> //necessario para limpar o terminal
#include <conio.h> //biblioteca de desenvolvimento
#include <string.h> //necessario para usar string
#define QTDCARACTERES 60

/* Aluno: Rafael Bortolozo 
   Arvore AVL
*/

typedef struct sBook{
	char name[QTDCARACTERES];
	int issn;
}Book;

typedef struct sNodo{
	struct sNodo* left;
	struct sNodo* right;
	Book* book;
	int height;
}Nodo;

Nodo* createNodo();					 			    //cria nodo
Book* createBook();					 			    //cria livro
Nodo* insertNode(Nodo* root, char* name, int issn); //insere livro
Nodo* avlInsert(Nodo* root, int issn);				//AVL no insert()
char* requestName();				 			    //solicita nome do livro
int requestIssn();					 		        //solicita ISSN do livro
Nodo* deleteNode(Nodo* root, int issn);	 			//delete livro
Nodo* avlDelete(Nodo* root);						//AVL no delete()
Nodo* search(Nodo* root, int issn);  			    //pesquisa livro
void printSearch(Nodo* nodo);		 			    //imprime pesquisa
Nodo* searchMax(Nodo* root);		 			    //Procura maior elemento
void preOrder (Nodo* root);			 			    //impressao Pre-ordem
void inOrder (Nodo* root);			 			    //impressao Em-ordem
void posOrder (Nodo* root);		 	 			    //impressao Pos-ordem
Nodo* free_tree(Nodo* root);		 			    //libera memoria
int max(int a, int b);				 			    //retorna o maior numero
int height(Nodo* N);         		 			    //retorna a altura da arvore
int getBalance(Nodo* root);			 			    //retorna o fator de balanceamento
Nodo* leftRotate(Nodo* root);		 			    //rotacao esquerda
Nodo* rightRotate(Nodo* root);		 			    //rotacao direita

main(){
	Nodo* root= NULL;
	Nodo* aux;
	char name[QTDCARACTERES];
	int issn;
	int op=0;
	
	while(op==0){
		printf("\n\t*****Sistema de biblioteca*****\n\n\n");
        printf("\t1)Inserir\n");
        printf("\t2)Excluir\n");
        printf("\t3)Buscar\n");
        printf("\t4)Pre-ordem\n");
        printf("\t5)Em-ordem\n");
        printf("\t6)Pos-ordem\n");
        printf("\t7)Limpar sistema\n");
        printf("\t8)Sair\n");
        
        scanf("%d",&op);
        switch (op){
            case 1: system("clear||cls");
					strcpy(name, requestName());
					issn= requestIssn();
					system("clear||cls");
					root= insertNode(root, name, issn);
					printf("\tLivro cadastrado com sucesso!\n\n");
					op=0;
                    break;
			
			case 2: system("clear||cls");
					issn= requestIssn();
					system("clear||cls");
					aux=search(root, issn);
					if(aux!=NULL){
						root= deleteNode(root, issn);
						printf("\tLivro \"%d\" removido!", issn);
					}else{
						printf("\tLivro \"%d\" nao encontrado!", issn);
					}
                    op=0;
                    printf("\n\n");
					break;
                    
			case 3: system("clear||cls");
					issn= requestIssn();
					system("clear||cls");
					aux= search(root, issn);
					printSearch(aux);
					op=0;
					break;
			
			case 4: system("clear||cls");
					printf("\tPre-ordem: \n");
					preOrder(root);
					printf("\n\n");
					op=0;
					break;
			
			case 5: system("clear||cls");
					printf("\tEm-ordem: \n");
					inOrder(root);
					printf("\n\n");
					op=0;
					break;
					
			case 6: system("clear||cls");
					printf("\tPos-ordem: \n");
					posOrder(root);
					printf("\n\n");
					op=0;
					break;
					
			case 7: system("clear||cls");
					printf("Tem certeza que deseja excluir todos os livros? (1-SIM, 2-NAO)\n");
					scanf("%d", &op);
					system("clear||cls");
					if(op==1){
						root=free_tree(root);
						printf("\tTodos os livros foram excluidos!\n\n");
					} 
					op=0;
					break;
					
			case 8: root=free_tree(root);
					break;		
               
            default:system("clear||cls");
					op=0;
                    break;
        }
    }
}

Nodo* createNodo(){
	Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->left = NULL;
    nodo->right = NULL;
    nodo->book = createBook();
    nodo->height=1;
    return nodo;
}

Book* createBook(){
	Book* book=(Book*)malloc(sizeof(Book));
	book->issn= NULL;
	return book;
}

Nodo* insertNode(Nodo *root, char* name, int issn){
    
	//Caso encontrar um espaco vazio, o nodo eh inserido e finaliza o codigo
	if(root == NULL){
        Nodo* aux= createNodo();
        strcpy(aux->book->name, name);
		aux->book->issn = issn;
        return aux;
    }
    
    //caso nao foi encontrado um espaco vazio, continua a percorrer a arvore atraves da recursividade
    //percorre a arvore atraves do insert, ate encontrar um ponteiro LEFT ou RIGHT que seja NULL
    if(issn <= root->book->issn) root->left = insertNode(root->left, name, issn); 
	else if(issn > root->book->issn) root->right = insertNode(root->right, name, issn);
	else return root;
	
	//apos a insercao...
	root= avlInsert(root, issn); //aplicacao da AVL
	return root;
}

Nodo* avlInsert(Nodo* root, int issn){
	//atualiza altura dos nodos
	root->height = 1 + max(height(root->left), height(root->right));
	
	//obtem o fator de balanceamento do nodo
	int balance= getBalance(root);
	
	//se nao for balanceado, sera aplicado a rotacao adequada
	// Left Left Case 
    if (balance > 1 && issn < root->left->book->issn) 
        return rightRotate(root); 
  
    // Right Right Case 
    if (balance < -1 && issn > root->right->book->issn) 
        return leftRotate(root); 
  
    // Left Right Case 
    if (balance > 1 && issn > root->left->book->issn){ 
        root->left =  leftRotate(root->left); 
        return rightRotate(root); 
    } 
  
    // Right Left Case 
    if (balance < -1 && issn < root->right->book->issn){ 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    }
    
    return root;
}

//solicita o nome do livro
char* requestName(){
	char name[QTDCARACTERES];
	printf("Digite o nome do livro: ");
	scanf(" %[^\n]", &name);
	return name;
}

//solicita o ISSN do livro
int requestIssn(){
	int issn;
	printf("Digite o ISSN do livro: ");
	scanf("%d", &issn);
	return issn;
}

Nodo* deleteNode(Nodo* root, int issn){
    //procurando o elemento
	//avanca para a subarvore da direita 
    if (issn < root->book->issn) root->left = deleteNode(root->left, issn); 
  
    //avanca para a subarvore da esquerda 
    else if(issn > root->book->issn ) root->right = deleteNode(root->right, issn); 
  	
  	//elemento encontrado
    else{ 
        //nodo com apenas 1 filho ou nenhum
        if((root->left == NULL) || (root->right == NULL)){ 
            Nodo *temp = root->left ? root->left : root->right; 
  
            //nenhum filho 
            if (temp == NULL){ 
                temp = root; 
                root = NULL; 
            }
			
			//apenas 1 filho
			else *root = *temp;
            free(temp); 
        }else{ 
            //nodo com 2 filhos, procura o maior elemento da subarvore da esquerda 
            Nodo* temp = searchMax(root->left); 
  
            //faz uma copia do elemento recem-procurado, passando para o root
            root->book->issn = temp->book->issn;
			strcpy(root->book->name, temp->book->name); 
  
            //delete o elemento procurado
            root->left = deleteNode(root->left, temp->book->issn); 
        } 
    }
    
    //apos a exclusao...
    if (root == NULL) return root; //se a arvore ficou vazia, entao retorne 
    root= avlDelete(root); //aplicacao da AVL
    return root; 
}

Nodo* avlDelete(Nodo* root){
	//atualiza altura dos nodos
    root->height = 1 + max(height(root->left), height(root->right)); 
  
    //obtem o fator de balanceamento do nodo
    int balance = getBalance(root);
	
	//se nao for balanceado, sera aplicado a rotacao adequada
	// Left Left Case 
    if (balance > 1 && getBalance(root->left) >= 0) 
        return rightRotate(root); 
  
    // Left Right Case 
    if (balance > 1 && getBalance(root->left) < 0){ 
        root->left =  leftRotate(root->left); 
        return rightRotate(root); 
    } 
  
    // Right Right Case 
    if (balance < -1 && getBalance(root->right) <= 0) 
        return leftRotate(root); 
  
    // Right Left Case 
    if (balance < -1 && getBalance(root->right) > 0){ 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    }
    
    return root;
}

Nodo* search(Nodo* root, int issn){
	//se nao encontrar o livro, retorna NULL
	if(root==NULL) return NULL;
	
	if(root->book->issn == issn) return root; //se encontrou, vai retornar o elemento
	
	//se nao, percorre a arvore usando recursividade
	else if(issn <= root->book->issn) root=search(root->left, issn);
	else root=search(root->right, issn);
	
	return root;
}

void printSearch(Nodo* nodo){
	if(nodo==NULL){
		printf("\tLivro inexistente!\n\n");
	}else{
		printf("\tLivro encontrado!\n");
		printf("\t%d- \"%s\" \n\n",nodo->book->issn, nodo->book->name);
	}
}

Nodo* searchMax(Nodo* root){
    Nodo* aux = root;
    while(aux->right != NULL){
        aux = aux->right;
    }
    return aux;
}

void preOrder (Nodo* root){
 	if(root != NULL){
	    printf("\n\t%d- \"%s\" ",root->book->issn, root->book->name);
		preOrder(root->left);
		preOrder(root->right);	    
 	}
}

void inOrder (Nodo* root){
 	if(root != NULL){
	    inOrder(root->left);
	    printf("\n\t%d- \"%s\" ",root->book->issn, root->book->name);
		inOrder(root->right);	    
 	}
}

void posOrder (Nodo* root){
 	if(root != NULL){
	    posOrder(root->left);
	    posOrder(root->right);
	    printf("\n\t%d- \"%s\" ",root->book->issn, root->book->name);
 	}
}

Nodo* free_tree(Nodo* root){
	//algoritmo do pos_ordem
	//free() no lugar de printf()
	if(root != NULL){
	    free_tree(root->left);
	    free_tree(root->right);
	    free(root);
 	}
 	return NULL;
}

//retorna o maior valor
int max(int a, int b){
	return (a > b)? a : b;
}

//retorna a altura do nodo
int height(Nodo* node){
	if (node == NULL){
		return 0;
	}  
    return node->height;
} 

//calcula o balanceamento
int getBalance(Nodo* root){ 
    if (root == NULL){
    	return 0;
	}
    return height(root->left) - height(root->right); 
} 

Nodo* rightRotate(Nodo *root){ 
    Nodo *x = root->left; 
    Nodo *T2 = x->right; 
  
    //rotacao
    x->right = root; 
    root->left = T2; 
  
    //atualiza alturas 
    root->height =1+ max(height(root->left), height(root->right)); 
    x->height =1+ max(height(x->left), height(x->right)); 
  
    //retorna o novo root 
    return x; 
}

Nodo* leftRotate(Nodo *root){ 
    Nodo *y = root->right; 
    Nodo *T2 = y->left; 
  
    //rotacao 
    y->left = root; 
    root->right = T2; 
  
    //atualiza alturas 
    root->height =1+ max(height(root->left), height(root->right)); 
    y->height =1+ max(height(y->left), height(y->right)); 
  
    //retorna o novo root  
    return y; 
} 
