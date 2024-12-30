#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#ifndef BIBLIOTECA_H_
#define BIBLIOTECA_H_

#define ALEATORIO(x) rand() % (x)
#define tamTabela 100
#define funcaoHash(x) (x) % tamTabela
#define MAX_AMIGOS 10

typedef struct amigo amigo;
struct amigo {
	int idAmigo;
	struct amigo *proximo;
};

typedef struct conta conta;
struct conta {
	int id;
	char usuario[50];
	float saldo;
	amigo *amigos;
	conta *proximo;
};

typedef struct arrayHash arrayHash;
struct arrayHash {
	conta *account;
	int qtde;
};

void menu();
void criarConta(arrayHash *tabela);
conta* procurarConta(arrayHash *tabela, int id);
conta* procurarContaPorUsuario(arrayHash *tabela, char *usuario);
int gerarIdUnico(arrayHash *tabela);
void entrarConta(arrayHash *tabela);
void excluirConta(arrayHash *tabela);
void menuDeJogos(conta *c, arrayHash *tabela);
void jogoCaraOuCoroa(conta *c);
void jogoDeDados(conta *c);
void jogoRoleta(conta *c);
void jogo21(conta *c);
void sairConta();
void adicionarAmizade(arrayHash *tabela, int id1, int id2);
void listarAmigos(arrayHash *tabelaContas, conta *c);

#endif
