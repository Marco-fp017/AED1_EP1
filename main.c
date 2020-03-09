#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0
#define IDADEPREFERENCIAL 60

typedef int bool;

typedef struct aux {
  int id;
  int idade;
  struct aux* prox;
} ELEMENTO , * PONT;

typedef struct {
  PONT inicio;
  PONT fimPref;
  PONT inicioNaoPref;
  PONT fim;
} FILAPREFERENCIAL, * PFILA;

//#include "filapreferencial.h"

PFILA criarFila(){
	PFILA res = (PFILA) malloc(sizeof(FILAPREFERENCIAL));
	res->inicio = NULL;
	res->fimPref = NULL;
	res->inicioNaoPref = NULL;
	res->fim = NULL;
	return res;
}

int tamanho(PFILA f){
	PONT atual = f->inicio;
	int tam = 0;
	while (atual) {
		atual = atual->prox;
		tam++;
	}
	return tam;
}

PONT buscarID(PFILA f, int id){
	PONT atual = f->inicio;
	 while (atual) {
		if (atual->id == id) return atual;
		atual = atual->prox;
	}
	return NULL;
}


void exibirLog(PFILA f){
	int numElementos = tamanho(f);
	printf("\nLog fila [elementos: %i] - Inicio:", numElementos);
	PONT atual = f->inicio;
	while (atual){
		printf(" [%i;%i]", atual->id, atual->idade);
		atual = atual->prox;
	}
	printf("\n\n");
}


int consultarIdade(PFILA f, int id){
	PONT atual = f->inicio;
	 while (atual) {
		if (atual->id == id) return atual->idade;
		atual = atual->prox;
	}
	return -1;
}


bool inserirPessoaNaFila(PFILA f, int id, int idade){
	if ((id < 0) || (idade < 0) || (buscarID(f, id) != NULL)) return false;
	PONT newPessoa = (PONT) malloc(sizeof(ELEMENTO));
		newPessoa->id = id;	
		newPessoa->idade = idade;
		
	if (f->inicio == NULL){
		if(idade >= IDADEPREFERENCIAL){
			//primeira pessoa caso seja idosa
			f->inicio = newPessoa;
			f->fimPref = newPessoa;
			f->fim = newPessoa;
			f->inicioNaoPref = NULL;
			newPessoa->prox = NULL;
		} else {
			//primeira pessoa caso não seja idosa
			f->inicio = newPessoa;
			f->inicioNaoPref = newPessoa;
			f->fim = newPessoa;
			f->fimPref = NULL;
			newPessoa->prox = NULL;
		}
	
	} else if (f->inicio != NULL){
		if(idade >= IDADEPREFERENCIAL){
			//entrada na fila preferencial
			if (f->fimPref != NULL){
			//entrada na fila preferencial não vazia => Adicionar um idoso no final da fila preferencial, pois já existe um idoso lá
				if (f->inicioNaoPref != NULL) newPessoa->prox = f->inicioNaoPref;
				else newPessoa->prox = NULL;
				f->fimPref->prox = newPessoa;
				f->fimPref = newPessoa;
				if (f->inicioNaoPref == NULL) f->fim = newPessoa;				
			} else {
			//entrada na fila preferencial vazia => Adicionar o primeiro idoso na fila preferencial, pois não há nenhum idoso lá
				f->inicio = newPessoa;
				newPessoa->prox = f->inicioNaoPref;
				f->fimPref = newPessoa;				
			}			
		} else{
			//entrada na fila não preferencial 
			if(f->inicioNaoPref == NULL){
				//fila não preferencial vazia
				f->inicioNaoPref = newPessoa;
				f->fimPref->prox = f->inicioNaoPref;
				f->fim = newPessoa;
				newPessoa->prox = NULL;
			} else { 
				f->fim->prox = newPessoa;
				f->fim = newPessoa;
				newPessoa->prox = NULL;
			}
		}
	}
	return true;
}

bool atenderPrimeiraDaFila(PFILA f, int* id){
	if (f->inicio == NULL) return false;
	PONT apagar = f->inicio;
	*id = f->inicio->id;

	if ((f->inicio != f->inicioNaoPref) && (f->inicio->prox != f->inicioNaoPref)){
		//O 1° da fila é preferencial e o próximo também é
		if (f->inicio->prox == f->fimPref) f->inicio = f->fimPref;
		else f->inicio = f->inicio->prox;
		free(apagar);

	} else if (f->inicio == f->fimPref && f->inicio != f->inicioNaoPref){
		//Só tem uma pessoa com atendimento preferencial na fila, o próxmo não é
		f->inicio = f->inicioNaoPref;
		//f->inicio->prox = f->inicioNaoPref->prox;
		f->fimPref = NULL;
		free(apagar);

	}else if ((f->inicio == f->inicioNaoPref) && (f->fimPref == NULL)){
		//Só tem pessoas não preferenciais na fila
		if (f->inicio == f->fim) {
		  free(apagar);
		  criarFila();
		}else {
		f->inicio = f->inicioNaoPref->prox;
			//f->inicioNaoPref = f->inicioNaoPref->prox;
			free(apagar);
		}

	} 
	return true;
}


PONT buscarAnt(PFILA f, int id){
    PONT atual = f->inicio->prox;
    PONT anterior = f->inicio;
    while (atual){
      if(atual->id == id) return anterior;
      anterior = anterior->prox;
      atual = atual->prox;
    }
    return NULL;
}

bool desistirDaFila(PFILA f, int id){

	PONT desistente = buscarID(f, id);

	if (desistente == NULL) return false;
  PONT antDesistente;
	if (desistente != f->inicio) antDesistente = buscarAnt(f, desistente->id);
  else antDesistente = NULL;

  PONT apagar = desistente;

  if (f->inicio == f->fim){
    f->inicio = NULL;
    criarFila();
    return true;
  }

  if (desistente->idade >= IDADEPREFERENCIAL){
    //Desistente é preferencial

    /*else*/ if (desistente == f->inicio && desistente->prox->idade >= IDADEPREFERENCIAL){
      //Desistente é o 1° preferencial, e tem outro(s) preferencial(is) depois dele
      if(desistente->prox != f->fimPref) f->inicio = f->inicio->prox;
      else if (desistente->prox == f->fimPref) f->inicio = f->fimPref;
      else if (f->inicio == f->fim) criarFila();
    }
    else if (desistente == f->inicio && desistente == f->fimPref){
      //desistente é o 1° e o último preferencial
      if (desistente->prox == f->inicioNaoPref){
         f->inicio = f->inicioNaoPref;
        f->fimPref = NULL;
      }else criarFila();
    }
    else if (desistente != f->inicio && desistente == f->fimPref){
      //desistente não é o 1° porém é o último (PRECISO ACERTAR OS PONTEIROS DO ELEMENTO ANTERIOR?)
      if (antDesistente == f->inicio && f->inicioNaoPref != NULL){
        //anterior é o 1° elemento e existem elementos não preferenciais
        f->inicio->prox = f->inicioNaoPref;
        f->fimPref = f->inicio;
      } else if (antDesistente != f->inicio && f->inicioNaoPref != NULL){
        //anterior NÃO é o 1° elemento e existem elementos não preferenciais
        antDesistente->prox = f->inicioNaoPref;
        f->fimPref = antDesistente;
      } else if (antDesistente == f->inicio && f->inicioNaoPref == NULL){
        //anterior é o 1° elemento e NÃO existem elementos não preferenciais
        f->fimPref = f->inicio;
        f->fim = f->inicio;
        antDesistente->prox = NULL;
      } else if (antDesistente != f->inicio && f->inicioNaoPref == NULL){
        //anterior NÃO é o 1° elemento e  NÃO existem elementos não preferenciais
        antDesistente = f->fimPref;
        antDesistente = f->fim;
      }
    }
    else{
      //desistente tá no meio da fila preferencial
      if (f->inicio == f->fim) criarFila();
      else if (desistente->prox == f->fimPref && f->inicio->prox == desistente){
        //anterior é o 1° e o próximo é o último preferencial
        f->inicio->prox = f->fimPref;
        desistente = f->fimPref;
      } 
      else if (desistente->prox == f->fimPref && f->inicio->prox != desistente){
        //anterior NÃO é o 1° e o próximo é o último preferencial
        antDesistente->prox = f->fimPref;
        desistente = f->fimPref;
      } 
      else if (f->inicio->prox == desistente && desistente->prox != f->fimPref){
        //anterior é o 1° e o próximo NÃO é o último preferencial
        f->inicio->prox = desistente->prox;
        desistente = desistente->prox;
      }
      else if (desistente->prox != f->fimPref && f->inicio->prox != desistente){
        //anterior NÃO é o 1° e o próximo NÃO é o último preferencial
        antDesistente->prox = desistente->prox;
        desistente = desistente->prox;
      }
      
    }
  }
  else {
    //Desistente NÃO é preferencial

    /*else*/ if (desistente == f->inicioNaoPref && desistente == f->inicio){
      if (desistente->prox == f->fim) f->inicio = f->fim;
      else f->inicio = f->inicio->prox; 
    }
    else if(desistente == f->fim){
      //é o último elemento mas nem ele, nem seu anterior são o primeiro
      //antDesistente = f->fim;
      if(antDesistente == f->inicio){
         f->fim = f->inicio;
      }
      else if (antDesistente == f->inicioNaoPref){
        //f->inicioNaoPref = f->fim;
        f->fim = antDesistente;
      }
      else if (antDesistente == f->fimPref){
        f->fim = f->fimPref;
        f->inicioNaoPref = NULL; 
      }
      else f->fim = antDesistente;
      antDesistente->prox = NULL;
    }
    else if (desistente == f->inicioNaoPref && desistente != f->inicio){
      //Se ele for o 1° NÃO preferencial
      if (antDesistente != NULL && desistente->prox != NULL) antDesistente->prox = desistente->prox;
      else antDesistente->prox = NULL;

      if (f->inicioNaoPref->prox != f->fim) f->inicioNaoPref = f->inicioNaoPref->prox;
      else if (f->inicioNaoPref->prox == f->fim) f->inicioNaoPref = f->fim;
      else if (f->inicioNaoPref == f->fim) {
        f->fimPref = f->fim;
        f->inicioNaoPref = NULL;
      }
    }

    else{
      //está no meio da fila
      if (f->inicioNaoPref->prox == desistente && desistente->prox == f->fim){
        f->inicioNaoPref->prox = f->fim;
      } 
      else if (f->inicioNaoPref->prox == desistente && desistente->prox != f->fim){
        f->inicioNaoPref->prox = desistente->prox;
      } 
      else if (f->inicioNaoPref->prox != desistente && desistente->prox == f->fim){
        antDesistente->prox = f->fim;
      }
      else if (f->inicioNaoPref->prox != desistente && desistente->prox != f->fim){
        antDesistente->prox = desistente->prox;
      } 
    }
  }
	free(apagar);
	return true;
}

/*Exemplo de teste do programa*/

int main() {
	PFILA f = criarFila();
	int id;
	int idade;
	bool res;

	printf("################# INSERINDO #######################\n");

	exibirLog(f);
	res = inserirPessoaNaFila(f, -1, -2);
	if(res) printf("Insercao retornou true (0).\n");
	else printf("Insercao retornou false (0). [OK]\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 1, 21);
	if(res) printf("Insercao retornou true (1). [OK]\n");
	else printf("Insercao retornou false (1).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 2, 11);
	if(res) printf("Insercao retornou true (2). [OK]\n");
	else printf("Insercao retornou false (2).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 3, 31);
	if(res) printf("Insercao retornou true (3). [OK]\n");
	else printf("Insercao retornou false (3).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 4, 71);
	if(res) printf("Insercao retornou true (4). [OK]\n");
	else printf("Insercao retornou false (4).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 5, 61);
	if(res) printf("Insercao retornou true (5). [OK]\n");
	else printf("Insercao retornou false (5).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 6, 81);
	if(res) printf("Insercao retornou true (6). [OK]\n");
	else printf("Insercao retornou false (6).\n");
	exibirLog(f);


	printf("################# ATENDENDO #######################\n");
	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (7), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (7).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (8), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (8).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (9), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (9).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (10), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (10).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (11), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (11).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (12), id=%i. [OK]\n",id);
	else printf("Atendimento retornou false (12).\n");

	exibirLog(f);
	res = atenderPrimeiraDaFila(f, &id);
	if(res) printf("Atendimento retornou true (13), id=%i.\n",id);
	else printf("Atendimento retornou false (13). [OK]\n");
	exibirLog(f);



	printf("################# INSERINDO PARTE 2 ###############\n");

	exibirLog(f);
	res = inserirPessoaNaFila(f, 7, 72);
	if(res) printf("Insercao retornou true (14). [OK]\n");
	else printf("Insercao retornou false (14).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 8, 22);
	if(res) printf("Insercao retornou true (15). [OK]\n");
	else printf("Insercao retornou false (15).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 9, 60);
	if(res) printf("Insercao retornou true (16). [OK]\n");
	else printf("Insercao retornou false (16).\n");
	exibirLog(f);
	res = inserirPessoaNaFila(f, 10, 42);
	if(res) printf("Insercao retornou true (17). [OK]\n");
	else printf("Insercao retornou false (17).\n");
	exibirLog(f);


	printf("################# SAINDO DA FILA ##################\n");

	exibirLog(f);
	res = desistirDaFila(f, 6);
	if(res) printf("Desistindo da fila retornou true (18).\n");
	else printf("Desistindo da fila retornou false (18). [OK]\n");
	exibirLog(f);
	res = desistirDaFila(f,10);
	if(res) printf("Desistindo da fila retornou true (19). [OK]\n");
	else printf("Desistindo da fila retornou false (19).\n");
	exibirLog(f);
	res = desistirDaFila(f,9);
	if(res) printf("Desistindo da fila retornou true (20). [OK]\n");
	else printf("Desistindo da fila retornou false (20).\n");
	exibirLog(f);
	res = desistirDaFila(f,8);
	if(res) printf("Desistindo da fila retornou true (21). [OK]\n");
	else printf("Desistindo da fila retornou false (21).\n");
	exibirLog(f);
	res = desistirDaFila(f,7);
	if(res) printf("Desistindo da fila retornou true (22). [OK]\n");
	else printf("Desistindo da fila retornou false (22).\n");
	exibirLog(f);

	return 0;
}