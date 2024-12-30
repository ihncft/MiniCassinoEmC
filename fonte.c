#include "biblioteca.h"

void menu() {
	arrayHash *tabelaContas = (arrayHash*) calloc(sizeof(arrayHash), tamTabela);
	char opcao;
	do {
		printf(
				"Digite a opção desejada:\nA - Criar Conta\nB - Entrar\nC - Excluir Conta\nD - Adicionar Amigo\nE - Finalizar\n");
		scanf(" %c", &opcao);
		opcao = toupper(opcao);
		switch (opcao) {
		case 'A':
			criarConta(tabelaContas);
			break;
		case 'B':
			entrarConta(tabelaContas);
			break;
		case 'C':
			excluirConta(tabelaContas);
			break;
		case 'D': {
			int id1, id2;
			printf("Digite o ID da sua conta: ");
			scanf("%d", &id1);
			printf("Digite o ID da conta que deseja adicionar como amigo: ");
			scanf("%d", &id2);
			adicionarAmizade(tabelaContas, id1, id2);
			break;
		}
		case 'E':
			break;
		default:
			printf("Opção inválida!\n");
		}
	} while (opcao != 'E');
}

void criarConta(arrayHash *tabela) {
	char usuario[50];
	bool valido;
	conta *novaConta = (conta*) malloc(sizeof(conta));

	if (!novaConta) {
		printf("Erro de alocação de memória\n");
		return;
	}

	do {
		valido = true;
		printf("Digite o nome do usuário: ");
		scanf(" %49s", usuario);

		if (strlen(usuario) < 4 || strlen(usuario) >= 50) {
			printf("O usuário deve ter mais que 3 e menos que 50 caracteres\n");
			valido = false;
		}

		for (int i = 0; usuario[i] != '\0'; i++) {
			char c = usuario[i];
			if (!isalnum(c) && c != '_' && c != '-') {
				printf("Utilize apenas caracteres válidos!\n");
				valido = false;
				break;
			}
		}
	} while (!valido);

	strcpy(novaConta->usuario, usuario);
	novaConta->saldo = 100;
	novaConta->id = gerarIdUnico(tabela);
	novaConta->amigos = NULL;

	int hash = funcaoHash(novaConta->id);
	novaConta->proximo = tabela[hash].account;
	tabela[hash].account = novaConta;
	tabela[hash].qtde++;
	printf("Conta criada com sucesso! ID da conta: %d\n", novaConta->id);
}

conta* procurarConta(arrayHash *tabela, int id) {
	int indice = funcaoHash(id);
	conta *atual = tabela[indice].account;

	while (atual) {
		if (atual->id == id) {
			return atual;
		}
		atual = atual->proximo;
	}
	return NULL;
}

conta* procurarContaPorUsuario(arrayHash *tabela, char *usuario) {
	for (int i = 0; i < tamTabela; i++) {
		conta *atual = tabela[i].account;
		while (atual) {
			if (strcmp(atual->usuario, usuario) == 0) {
				return atual;
			}
			atual = atual->proximo;
		}
	}

	return NULL;
}

int gerarIdUnico(arrayHash *tabela) {
	int id;
	do {
		id = ALEATORIO(1000);
	} while (procurarConta(tabela, id) != NULL);
	return id;
}

void entrarConta(arrayHash *tabela) {
	char usuario[50];
	printf("Digite o nome de usuário: ");
	scanf(" %49s", usuario);

	conta *c = procurarContaPorUsuario(tabela, usuario);
	if (c) {
		printf("Bem-vindo, %s! Seu saldo é: %.2f\n", c->usuario, c->saldo);
		menuDeJogos(c, tabela);
	} else {
		printf("Conta não encontrada.\n");
	}
}

void excluirConta(arrayHash *tabela) {
	char usuario[50];
	printf("Digite o nome do usuário a ser excluído: ");
	scanf(" %49s", usuario);

	for (int i = 0; i < tamTabela; i++) {
		conta *atual = tabela[i].account;
		conta *anterior = NULL;

		while (atual) {
			if (strcmp(atual->usuario, usuario) == 0) {
				if (!anterior) {
					tabela[i].account = atual->proximo;
				} else {
					anterior->proximo = atual->proximo;
				}

				free(atual);
				tabela[i].qtde--;
				printf("Conta de %s excluída com sucesso.\n", usuario);
				return;
			}
			anterior = atual;
			atual = atual->proximo;
		}
	}
	printf("Conta não encontrada.\n");
}

void jogoCaraOuCoroa(conta *c) {
	int aposta, escolha, resultado;
	printf("Digite o valor da aposta: ");
	scanf("%d", &aposta);

	if (aposta > c->saldo) {
		printf("Saldo insuficiente.\n");
		return;
	}

	printf("Escolha: 1 para Cara, 2 para Coroa: ");
	scanf("%d", &escolha);

	resultado = ALEATORIO(2) + 1;
	if (escolha == resultado) {
		c->saldo += aposta;
		printf("Parabéns! Você ganhou. Seu novo saldo é: %.2f\n", c->saldo);
	} else {
		c->saldo -= aposta;
		printf("Você perdeu. Seu novo saldo é: %.2f\n", c->saldo);
	}
}

void jogoDeDados(conta *c) {
	int aposta, escolha, resultado;
	printf("Digite o valor da aposta: ");
	scanf("%d", &aposta);

	if (aposta > c->saldo) {
		printf("Saldo insuficiente.\n");
		return;
	}

	printf("Escolha um número entre 1 e 6: ");
	scanf("%d", &escolha);

	if (escolha < 1 || escolha > 6) {
		printf("Escolha inválida.\n");
		return;
	}

	resultado = ALEATORIO(6) + 1;
	printf("O dado caiu em: %d\n", resultado);

	if (escolha == resultado) {
		c->saldo += aposta * 5;
		printf("Parabéns! Você ganhou. Seu novo saldo é: %.2f\n", c->saldo);
	} else {
		c->saldo -= aposta;
		printf("Você perdeu. Seu novo saldo é: %.2f\n", c->saldo);
	}
}

void jogoRoleta(conta *c) {
	int aposta, escolha, resultado;
	printf("Digite o valor da aposta: ");
	scanf("%d", &aposta);

	if (aposta > c->saldo) {
		printf("Saldo insuficiente.\n");
		return;
	}

	printf("Escolha: 1 para Vermelho, 2 para Preto: ");
	scanf("%d", &escolha);

	resultado = ALEATORIO(2) + 1;
	if (escolha == resultado) {
		c->saldo += aposta * 1.5;
		printf("Parabéns! Você ganhou. Seu novo saldo é: %.2f\n", c->saldo);
	} else {
		c->saldo -= aposta;
		printf("Você perdeu. Seu novo saldo é: %.2f\n", c->saldo);
	}
}

void jogo21(conta *c) {
	int aposta, usuarioPontos = 0, cassinoPontos = 0;
	printf("Digite o valor da aposta: ");
	scanf("%d", &aposta);

	if (aposta > c->saldo) {
		printf("Saldo insuficiente.\n");
		return;
	}

	usuarioPontos = 16 + ALEATORIO(6);
	cassinoPontos = 16 + ALEATORIO(6);

	printf("Sua pontuação: %d\n", usuarioPontos);
	printf("Pontuação do cassino: %d\n", cassinoPontos);

	if (usuarioPontos > cassinoPontos) {
		c->saldo += aposta * 2;
		printf("Parabéns! Você venceu. Seu novo saldo é: %.2f\n", c->saldo);
	} else if (usuarioPontos < cassinoPontos) {
		c->saldo -= aposta;
		printf("Você perdeu. Seu novo saldo é: %.2f\n", c->saldo);
	} else {
		printf("Empate! Seu saldo permanece em: %.2f\n", c->saldo);
	}
}

void sairConta() {
	printf("Você saiu da conta com sucesso.\n");
}

void menuDeJogos(conta *c, arrayHash *tabela) {
	int opcao;
	do {
		printf("Escolha uma opção:\n");
		printf("1 - Cara ou Coroa\n");
		printf("2 - Jogo de Dados\n");
		printf("3 - Roleta\n");
		printf("4 - Jogo de 21 (Blackjack)\n");
		printf("5 - Listar Amigos\n");
		printf("6 - Sair da Conta\n");
		printf("Digite a opção desejada: ");
		scanf("%d", &opcao);

		switch (opcao) {
		case 1:
			jogoCaraOuCoroa(c);
			break;
		case 2:
			jogoDeDados(c);
			break;
		case 3:
			jogoRoleta(c);
			break;
		case 4:
			jogo21(c);
			break;
		case 5:
			listarAmigos(tabela, c);
			break;
		case 6:
			sairConta();
			return;
		default:
			printf("Opção inválida! Tente novamente.\n");
		}
	} while (opcao != 6);
}

void adicionarAmizade(arrayHash *tabela, int id1, int id2) {
	conta *conta1 = procurarConta(tabela, id1);
	conta *conta2 = procurarConta(tabela, id2);

	if (!conta1 || !conta2) {
		printf("Uma das contas não foi encontrada.\n");
		return;
	}

	amigo *novoAmigo1 = (amigo*) malloc(sizeof(amigo));
	novoAmigo1->idAmigo = id2;
	novoAmigo1->proximo = conta1->amigos;
	conta1->amigos = novoAmigo1;

	amigo *novoAmigo2 = (amigo*) malloc(sizeof(amigo));
	novoAmigo2->idAmigo = id1;
	novoAmigo2->proximo = conta2->amigos;
	conta2->amigos = novoAmigo2;

	printf("Amizade adicionada entre %s e %s.\n", conta1->usuario,
			conta2->usuario);
}

void listarAmigos(arrayHash *tabelaContas, conta *c) {
	if (!c->amigos) {
		printf("%s ainda não tem amigos.\n", c->usuario);
		return;
	}

	printf("Amigos de %s:\n", c->usuario);
	amigo *atual = c->amigos;
	while (atual) {
		conta *amigoConta = procurarConta(tabelaContas, atual->idAmigo);
		if (!amigoConta) {
			printf("Amigo com ID %d não encontrado.\n", atual->idAmigo);
		} else {
			printf("- %s (ID: %d)\n", amigoConta->usuario, amigoConta->id);
		}
		atual = atual->proximo;
	}
}

