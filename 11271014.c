#include "filapreferencial.h"

PFILA criarFila(){
    PFILA res = (PFILA) malloc(sizeof(FILAPREFERENCIAL));
    res->cabeca = (PONT) malloc(sizeof(ELEMENTO));
    res->inicioNaoPref = res->cabeca;
    res->cabeca->id = -1;
    res->cabeca->idade = -1;
    res->cabeca->ant = res->cabeca;
    res->cabeca->prox = res->cabeca;
    return res;
}

int tamanho(PFILA f){
	PONT atual = f->cabeca->prox;
	int tam = 0;
	while (atual != f->cabeca) {
		atual = atual->prox;
		tam++;
	}
	return tam;
}

PONT buscarID(PFILA f, int id){
	PONT atual = f->cabeca->prox;
	while (atual != f->cabeca) {
		if (atual->id == id) return atual;
		atual = atual->prox;
	}
	return NULL;
}

void exibirLog(PFILA f){
	int numElementos = tamanho(f);
	printf("\nLog fila [elementos: %i]\t- Inicio:", numElementos);
	PONT atual = f->cabeca->prox;
	while (atual != f->cabeca) {
		printf(" [%i;%i]", atual->id, atual->idade);
		atual = atual->prox;
	}
	printf("\n                       \t-    Fim:");
	atual = f->cabeca->ant;
	while (atual != f->cabeca) {
		printf(" [%i;%i]", atual->id, atual->idade);
		atual = atual->ant;
	}
	printf("\n\n");
}


int consultarIdade(PFILA f, int id){
	PONT atual = f->cabeca->prox;
	while (atual != f->cabeca) {
		if (atual->id == id) return atual->idade;
		atual = atual->prox;
	}
	return -1;
}

bool inserirPessoaNaFila(PFILA f, int id, int idade){
  PONT busc = buscarID(f, id);
  if (id < 0 || idade < 0 || busc != NULL) return false;
  PONT novo = (PONT) malloc(sizeof(ELEMENTO));
  novo->id = id;
  novo->idade = idade;

  //inserção de pessoa preferencial
  if (novo->idade >= IDADEPREFERENCIAL){
    //fila está vazia
    if(f->cabeca->prox == f->cabeca){
      f->cabeca->prox = novo;
      f->cabeca->ant = novo;
      novo->prox = f->cabeca;
      novo->ant = f->cabeca;
    }
    //só tem idosos na fila
    else if (f->cabeca->prox != f->inicioNaoPref && f->inicioNaoPref == f->cabeca){
      novo->ant = f->cabeca->ant;
      novo->prox = f->cabeca;
      f->cabeca->ant->prox = novo;
      f->cabeca->ant = novo;
    }
    //tem idosos e pessoas não preferenciais na fila
    else if (f->cabeca->prox != f->inicioNaoPref && f->inicioNaoPref != f->cabeca){
      novo->prox = f->inicioNaoPref;
      novo->ant = f->inicioNaoPref->ant;
      f->inicioNaoPref->ant->prox = novo;
      f->inicioNaoPref->ant = novo;
    }
    //não tem idosos na fila
    else if (f->cabeca->prox == f->inicioNaoPref){
      novo->prox = f->inicioNaoPref;
      novo->ant = f->cabeca;
      f->inicioNaoPref->ant = novo;
      f->cabeca->prox = novo;
    }
    else return false;
  }
  //inserção de pessoa não preferencial
  else{
    //fila está vazia
    if(f->cabeca->prox == f->cabeca){
      f->inicioNaoPref = novo;
      novo->prox = f->cabeca;
      novo->ant = f->cabeca;
      f->cabeca->prox = novo;
      f->cabeca->ant = novo;
    }
    //só tem pessoas não preferenciais na fila
    else if (f->cabeca->prox == f->inicioNaoPref){
      novo->ant = f->cabeca->ant;
      novo->prox = f->cabeca;
      f->cabeca->ant->prox = novo;
      f->cabeca->ant = novo;
    }
    //só tem idosos na fila
    else if (f->cabeca->prox != f->inicioNaoPref && f->inicioNaoPref == f->cabeca){
      f->inicioNaoPref = novo;
      novo->prox = f->cabeca;
      novo->ant = f->cabeca->ant;
      f->cabeca->ant->prox = novo;
      f->cabeca->ant = novo;
    }
    //tem tanto idosos quanto pessoas não preferenciais na fila
    else if (f->cabeca->prox != f->inicioNaoPref && f->inicioNaoPref != f->cabeca){
      novo->ant = f->cabeca->ant;
      novo->prox = f->cabeca;
      f->cabeca->ant->prox = novo;
      f->cabeca->ant = novo;
    }
	else return false;
  }
  return true;
}

bool atenderPrimeiraDaFila(PFILA f, int* id){
  if (f->cabeca->prox == f->cabeca) return false;
  *id = f->cabeca->prox->id;
  PONT apagar = f->cabeca->prox;

  //só tem uma pessoa na fila
  if (apagar->prox == f->cabeca){
    f->cabeca->prox = f->cabeca;
    f->cabeca->ant = f->cabeca;
    f->inicioNaoPref = f->cabeca;
  }
  //Excluir uma pessoa que é preferencial e o seu próximo também
  else if (apagar->idade >= IDADEPREFERENCIAL && apagar->prox != f->inicioNaoPref){
      f->cabeca->prox = apagar->prox;
      apagar->prox->ant = f->cabeca;
    }
  //Excluir uma pessoa que é preferencial e o seu próximo não é preferencial
  else if (apagar->idade >= IDADEPREFERENCIAL && apagar->prox == f->inicioNaoPref){
      f->cabeca->prox = f->inicioNaoPref;
      f->inicioNaoPref->ant = f->cabeca;
    }

  //excluir uma pessoa não preferencial (neste caso só podem existir pessoas não preferenciais na fila)
  else if(apagar->idade < IDADEPREFERENCIAL){
    f->inicioNaoPref = apagar->prox;
    f->cabeca->prox = apagar->prox;
    f->inicioNaoPref->ant = f->cabeca;
  }
  else return false;
  free(apagar);
  return true;
}


bool desistirDaFila(PFILA f, int id){
  PONT desistente = buscarID(f, id);
  if(desistente == NULL) return false;

  //só tem uma pessoa na fila
  if(desistente->prox == f->cabeca && desistente->ant == f->cabeca){
    f->cabeca->prox = f->cabeca;
    f->cabeca->ant = f->cabeca;
    f->inicioNaoPref = f->cabeca;
  }

  //Desistente é preferencial
  else if (desistente->idade >= IDADEPREFERENCIAL){
    //primeiro da fila e o próximo são preferenciais
    if(desistente == f->cabeca->prox && desistente->prox != f->inicioNaoPref){
      f->cabeca->prox = desistente->prox;
      desistente->prox->ant = f->cabeca;
    }
    //Desistente é o 1° da fila, mas o seu próximo não é preferencial
    else if(desistente == f->cabeca->prox && desistente->prox == f->inicioNaoPref){
      f->cabeca->prox = f->inicioNaoPref;
      f->inicioNaoPref->ant = f->cabeca;
    }
    //Desistente no meio da fila e o seu próximo é preferencial
    else if (desistente->prox == f->inicioNaoPref){
      desistente->ant->prox = f->inicioNaoPref;
      f->inicioNaoPref->ant = desistente->ant;
    }
    //No meio da fila com próximo não preferencial
    else if (desistente != f->cabeca->ant){
      desistente->ant->prox = desistente->prox;
      desistente->prox->ant = desistente->ant;
    }
    //Desistente no final da fila 
    else if (f->cabeca->ant == desistente){
      desistente->ant->prox = f->cabeca;
      f->cabeca->ant = desistente->ant;
    }
    else return false;
  }

  //Desistente é não preferencial
  else{
    //Desistente é o 1° da fila 
    if(f->cabeca->prox == desistente){
      f->inicioNaoPref = f->inicioNaoPref->prox;
      f->cabeca->prox = f->inicioNaoPref;
      f->inicioNaoPref->ant = f->cabeca;
    }
    //desistente é o 1° não preferencial, mas com elementos preferenciais na fila
    else if (desistente == f->inicioNaoPref && desistente->ant != f->cabeca){
      // 1° não preferencial mas não é o último da fila
      if(desistente->prox != f->cabeca){
        desistente->prox->ant = f->inicioNaoPref->ant;
        f->inicioNaoPref->ant->prox = desistente->prox;
        f->inicioNaoPref = f->inicioNaoPref->prox;

      }
      // 1° não preferencial e o último da fila
      else if(desistente->prox == f->cabeca){
        f->inicioNaoPref->ant->prox = f->cabeca;
        f->cabeca->ant = f->inicioNaoPref->ant;
        f->inicioNaoPref->prox = f->cabeca;
      }
    }
    //Desistente tá no meio da fila
    else if (desistente != f->cabeca->prox && desistente != f->cabeca->ant){
      //Anterior do desistente é o 1° preferencial
      if(desistente->ant == f->inicioNaoPref ){
        f->inicioNaoPref->prox = desistente->prox;
        desistente->prox->ant = f->inicioNaoPref;
      }
      //Anterior do desistente não é o 1° preferencial e seu próximo é o último elemento
      else if (desistente->ant != f->inicioNaoPref){
        desistente->ant->prox = desistente->prox;
        desistente->prox->ant = desistente->ant;
      }
    }
    //Desistente é o último da fila
    else if (f->cabeca->ant == desistente){
      //Desistente é o último e o seu anterior é o 1° não pref.
      if(desistente->ant == f->inicioNaoPref){
        f->inicioNaoPref->prox = f->cabeca;
        f->cabeca->ant = f->inicioNaoPref;
      }
      //Desistente é o último e o seu anterior não é o 1° não pref.
      else if (desistente->ant != f->inicioNaoPref){
        desistente->ant->prox = f->cabeca;
        f->cabeca->ant = desistente->ant;
      }
    }
    else return false;
  }

  free(desistente);
  return true;
}
