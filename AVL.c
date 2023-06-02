#include<stdio.h>
#include<stdlib.h>

typedef struct TNoAVL{
    int chave;
    int bal;
    struct TNoAVL* esq;
    struct TNoAVL* dir;
}TNoAVL;

void rotEsq(TNoAVL **no){
    TNoAVL *raiz = *no;
    (*no)=(*no)->dir;
    raiz->dir = (*no)->esq;
    (*no)->esq = raiz;
}

void rotDir(TNoAVL **no){
    TNoAVL *raiz = (*no);
    (*no) = (*no)->esq;
    raiz->esq = (*no)->dir;
    (*no)->dir = raiz;
}

int inserir(TNoAVL **r, int chave){
    int cresceu;
    if(*r == NULL){
        //faz a insertion
        (*r) = malloc(sizeof(TNoAVL));
        (*r)->chave = chave;
        (*r)->bal = 0;
        (*r)->esq = NULL;
        (*r)->dir = NULL;
        cresceu=1;
    }
    else if(chave < (*r)->chave){
        cresceu = inserir(&((*r)->esq), chave);
        if(cresceu){
            switch((*r)->bal){
                case 1:
                (*r)->bal = 0;
                cresceu = 0;
                break;
                
                case 0:
                (*r)->bal = -1;
                cresceu = 1;
                break;

                case -1:
                //deve ser feita rotacao
                if((*r)->esq->bal <=0){
                    //rot simples dir
                    rotDir(r);
                    (*r)->bal = 0;
                    (*r)->dir->bal = 0;
                }
                else{
                    //rot dupla paizera
                    rotEsq(&((*r)->esq));
                    rotDir(r);
                    if((*r)->bal == -1){
                        (*r)->esq->bal = 0;
                        (*r)->dir->bal = 1;
                    }
                    else{
                    (*r)->dir->bal = 0;
                    (*r)->esq->bal = -(*r)->bal;
                    }
                    (*r)->bal = 0;
                }
                cresceu = 0;

            }
        }
    }
    else if(chave > (*r)->chave){
        cresceu = inserir(&((*r)->dir), chave);
        if(cresceu){
            switch((*r)->bal){
                case -1:
                    (*r)->bal = 0;
                    cresceu = 0;
                    break;
                case 0:
                    (*r)->bal = 1;
                    cresceu = 1;
                    break;
                case 1:
                    if((*r)->dir->bal >= 0){
                        //rot simples esq
                        rotEsq(r);
                        (*r)->esq->bal = 0;
                        (*r)->bal = 0;
                    }
                    else{
                        rotDir(&((*r)->dir));
                        rotEsq(r);
                        if((*r)->bal == -1){
                            (*r)->esq->bal = 0;
                            (*r)->dir->bal = 1;
                            
                        }
                        else{
                            (*r)->dir->bal = 0;
                            (*r)->esq->bal = -(*r)->bal;
                        }
                        (*r)->bal = 0;
                    }
                    cresceu = 0;
            }
        }
    }
    else
        cresceu = 0;
    return cresceu;
}

int getHeight(struct TNoAVL* root) {
    if (root == NULL) {
        return -1; // Height of an empty tree is -1
    }

    int leftHeight = getHeight(root->esq);
    int rightHeight = getHeight(root->dir);

    // Return the maximum of the left and right subtree heights, plus 1 for the root node
    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

TNoAVL **menorPP(TNoAVL **no){
    if(!(*no)){
        return NULL;
    }
    if((*no)->esq==NULL){
        return no;
    }
    else{
        menorPP(&((*no)->esq));
    }
}

int remover(TNoAVL **p, int chave){
    int decresceu;
    if((*p)->chave == chave){
        if((*p)->esq == NULL && (*p)->dir == NULL){
            free(*p);
            (*p)=NULL;
            decresceu = 1;
            return decresceu;
        }
        if(((*p)->esq == NULL) != ((*p)->dir == NULL)){
            TNoAVL *filho = ((*p)->esq == NULL) ? (*p)->dir : (*p)->esq;
            free(*p);
            (*p) = filho;
            decresceu = 1;
            return decresceu;
        }
        else{
            TNoAVL **substituto = menorPP(&((*p)->dir));
            (*p)->chave= (*substituto)->chave;
            remover(substituto, (*substituto)->chave);
            decresceu = 1;
            return decresceu;
        }

    }
    else if(chave<(*p)->chave){
        decresceu = remover(&((*p)->esq), chave);
        if(decresceu){
            switch((*p)->bal){
                case -1:
                    (*p)->bal = 0;
                    decresceu = 1;
                break;

                case 0:
                    (*p)->bal = 1;
                    decresceu = 0;
                break;

                case 1:
                if((*p)->dir->bal == 0){
                    //rot simples a esquerda
                    rotEsq(p);
                    (*p)->esq->bal = 1;
                    (*p)->bal = -1;
                }
                else if((*p)->dir->bal >= 1){
                    rotEsq(p);
                    (*p)->esq->bal = 0;
                    (*p)->bal = 0;
                }
                else{
                    rotDir(&((*p)->dir));
                    rotEsq(p);
                    if((*p)->bal == -1){
                        (*p)->esq->bal = 0;
                        (*p)->dir->bal = 1;
                    }
                    else{
                        (*p)->dir->bal = 0;
                        (*p)->esq->bal = -1;
                    }
                    (*p)->bal = 0;
                }
                decresceu = 1;
                
            }
        }
    }
    else if(chave > (*p)->chave){
        decresceu = remover(&((*p)->dir), chave);
        if(decresceu){
            switch((*p)->bal){
                case 1:
                    (*p)->bal=0;
                    decresceu=1;
                break;
                case 0:
                    (*p)->bal = -1;
                    decresceu = 0;
                break;
                case -1:
                    if((*p)->esq->bal==0){
                        rotDir(p);
                        (*p)->bal = 1;
                        (*p)->dir->bal = -1;
                    }
                    else if((*p)->esq->bal <= -1){
                        rotDir(p);
                        (*p)->bal = 0;
                        (*p)->dir->bal = 0;
                    }
                    else{
                        //rot dupla
                        rotEsq(&((*p)->esq));
                        rotDir(p);
                        if((*p)->bal == -1){
                            (*p)->esq->bal = 0;
                            (*p)->dir->bal = 1;
                        }
                        else{
                            (*p)->dir->bal = 0;
                            (*p)->esq->bal = -1;
                        }
                        (*p)->bal = 0;
                    }
                decresceu = 1;
                
            }
        }
    }
    else
        decresceu = 0;
    return decresceu;
}

void imprimir(TNoAVL *raiz, int nivel){
    int i;
    if(raiz){
        imprimir(raiz->dir, nivel +1);
        printf("\n\n");
        for(int i =0; i<nivel;i++){
            printf("\t");
        }
        printf("%d", raiz->chave);
        imprimir(raiz->esq, nivel +1);
    }
}


int main(){
    //3,2,1,7,4,5,0,10,20 & 30
    TNoAVL *raiz = NULL;
    inserir(&raiz, 3);
    inserir(&raiz, 2);
    inserir(&raiz, 1);
    inserir(&raiz, 7);
    inserir(&raiz, 4);
    inserir(&raiz, 5);
    inserir(&raiz, 0);
    inserir(&raiz, 10);
    inserir(&raiz, 20);
    inserir(&raiz, 30);
    remover(&raiz, 3);
    imprimir(raiz, 1);
    //printf("\nbal do no [%d] : [%d]\n", raiz->chave, raiz->bal);
    //printf("\nbal do no [%d] : [%d]\n", raiz->esq->chave, raiz->esq->bal);
    return 0;
}