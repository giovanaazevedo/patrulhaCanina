//Protótipo Giovana
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dados clientes
struct Cliente {
    char nome[50];
    int idade;
    char endereco[100];
    char profissao[50];
    char cpf[15];
    char rg[15];
};

// Funções
void solicitarDadosUsuario(char* linha);
int logarUsuario(FILE* arq, char valores[]);
int cadastrarUsuario(FILE* arq, char valores[]);
void incluirCliente();
void alterarCliente();
void excluirCliente();
void consultarCliente();

#define TAMUSUARIO 5
#define TAMSENHA 5
#define TAMLINHAUSUARIO TAMUSUARIO + 1 + TAMSENHA + 1

int main() {
    FILE *arqUsuario;
    char usuario[TAMUSUARIO + 1];
    int ehLogado = 0;

    printf("\nBem-vindo(a) ao Patrulha Canina!!!\n");

    int opcao;
    // REALIZA O LOGIN
    do {
        printf("\nMENU:\n");
        printf("1 - CADASTRAR USUÁRIO\n");
        printf("2 - LOGAR NO SISTEMA\n");
        printf("3 - SAIR\n");
        printf("O que deseja fazer? ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Para cadastrar um novo usuário é necessário estar logado no sistema. Tente novamente!\n");
                break;

            case 2: {
                arqUsuario = fopen("arquivos/usuarios.txt", "r");
                if (arqUsuario == NULL) {
                    printf("Erro interno. Tente novamente!\n");
                    opcao = 3;
                    break;
                }

                char dados[TAMLINHAUSUARIO];
                dados[0] = '\0';
                solicitarDadosUsuario(dados);
                int retorno = logarUsuario(arqUsuario, dados);
                if (retorno == 0)
                    printf("\nUsuário não cadastrado. Tente novamente!\n");
                else if (retorno == 1) {
                    for (int i = 0; i < strlen(dados); i++) {
                        if (dados[i] == ',')
                            break;

                        usuario[i] = dados[i];
                    }

                    printf("\nUsuário %s logado!\n", usuario);
                    opcao = 3;
                    ehLogado = 1;
                }

                fclose(arqUsuario);
            } break;

            case 3:
                break;

            default:
                printf("Opção inválida. Tente novamente!\n");
                break;
        }
    } while (opcao != 3);

    if (opcao == 3 && ehLogado == 0)
        return 0;

    // MENU SECUNDÁRIO
    do {
        int opcaoSub = 0;
        printf("\nMENU:\n");
        printf("1 - GERENCIAR CLIENTE\n");
        printf("2 - GERENCIAR ANIMAIS\n");
        printf("3 - GERENCIAR ADOÇÃO\n");
        printf("4 - CRIAR USUÁRIO FUNCIONÁRIO\n");
        printf("5 - VISUALIZAR ANIMAIS PARA ADOÇÃO\n");
        printf("6 - SAIR\n");
        printf("O que deseja fazer? ");
        scanf("%d", &opcaoSub);

        switch (opcaoSub) {
            case 1: {
                printf("1 - INCLUIR CLIENTE\n");
                printf("2 - ALTERAR CLIENTE\n");
                printf("3 - EXCLUIR CLIENTE\n");
                scanf("%d", &opcaoSub);

                if (opcaoSub == 1)
                    incluirCliente();
                else if (opcaoSub == 2)
                    alterarCliente();
                else if (opcaoSub == 3)
                    excluirCliente();
            } break;

            // ... (outros cases)

            case 6:
                break;

            default:
                printf("Opção inválida. Tente novamente!\n");
                break;
        }
    } while (opcaoSub != 6);

    return 0;
}

void solicitarDadosUsuario(char* linha) {
    int tamLinha = TAMUSUARIO + 1 + TAMSENHA + 1;
    char entrada[tamLinha];

    printf("\nDigite o seu usuário e senha, separados por espaço: ");
    fgets(entrada, tamLinha, stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    strcat(linha, entrada);
    strcat(linha, ",");
}

int logarUsuario(FILE* arq, char valores[]) {
    rewind(arq);
    char leitura[TAMLINHAUSUARIO];

    while (fgets(leitura, TAMLINHAUSUARIO, arq) != NULL) {
        leitura[strcspn(leitura, "\n")] = '\0';

        if (strcmp(leitura, valores) == 0)
            return 1;
    }

    return 0;
}

int cadastrarUsuario(FILE* arq, char valores[]) {
    rewind(arq);
    fprintf(arq, "\n%s", valores);
    fflush(arq);

    // Validar a inclusão verificando se o usuário está no arquivo após a inclusão
    rewind(arq);
    return logarUsuario(arq, valores);
}

void incluirCliente() {
    struct Cliente novoCliente;

    printf("\nDigite o nome completo do novo cliente: ");
    scanf(" %[^\n]", novoCliente.nome);

    printf("Digite a idade do novo cliente: ");
    scanf("%d", &novoCliente.idade);

    printf("Digite o endereço do novo cliente: ");
    scanf(" %[^\n]", novoCliente.endereco);

    printf("Digite a profissão do novo cliente: ");
    scanf(" %[^\n]", novoCliente.profissao);

    printf("Digite o CPF do novo cliente: ");
    scanf("%s", novoCliente.cpf);

    printf("Digite o RG do novo cliente: ");
    scanf("%s", novoCliente.rg);

    FILE *arqClientes = fopen("arquivos/clientes.txt", "a");
    if (arqClientes == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    fprintf(arqClientes, "%s,%d,%s,%s,%s,%s\n", novoCliente.nome, novoCliente.idade,
            novoCliente.endereco, novoCliente.profissao, novoCliente.cpf, novoCliente.rg);

    fclose(arqClientes);

    printf("\nCliente %s incluído com sucesso!\n", novoCliente.nome);
}

void alterarCliente() {
    FILE *arqClientes = fopen("arquivos/clientes.txt", "r");
    FILE *arqTemp = fopen("arquivos/temp.txt", "w");

    if (arqClientes == NULL || arqTemp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char cpfAlvo[15];
    printf("\nDigite o CPF do cliente a ser alterado: ");
    scanf("%s", cpfAlvo);

    struct Cliente clienteAtual;

    // Lê o arquivo original e escreve no arquivo temporário com as alterações
    while (fscanf(arqClientes, "%[^,],%d,%[^,],%[^,],%[^,],%[^\n]\n", clienteAtual.nome, &clienteAtual.idade,
                  clienteAtual.endereco, clienteAtual.profissao, clienteAtual.cpf, clienteAtual.rg) != EOF) {
        if (strcmp(clienteAtual.cpf, cpfAlvo) == 0) {
            // Cliente encontrado, solicita as novas informações
            printf("\nDigite o novo nome do cliente: ");
            scanf(" %[^\n]", clienteAtual.nome);

            printf("Digite a nova idade do cliente: ");
            scanf("%d", &clienteAtual.idade);

            printf("Digite o novo endereço do cliente: ");
            scanf(" %[^\n]", clienteAtual.endereco);

            printf("Digite a nova profissão do cliente: ");
            scanf(" %[^\n]", clienteAtual.profissao);

            // Mantém o CPF, pois não será alterado

            printf("Digite o novo RG do cliente: ");
            scanf("%s", clienteAtual.rg);
        }

        fprintf(arqTemp, "%s,%d,%s,%s,%s,%s\n", clienteAtual.nome, clienteAtual.idade,
                clienteAtual.endereco, clienteAtual.profissao, clienteAtual.cpf, clienteAtual.rg);
    }

    fclose(arqClientes);
    fclose(arqTemp);

    remove("arquivos/clientes.txt");
    rename("arquivos/temp.txt", "arquivos/clientes.txt");

    printf("\nCliente alterado com sucesso!\n");
}

void excluirCliente() {
    FILE *arqClientes = fopen("arquivos/clientes.txt", "r");
    FILE *arqTemp = fopen("arquivos/temp.txt", "w");

    if (arqClientes == NULL || arqTemp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char cpfAlvo[15];
    printf("\nDigite o CPF do cliente a ser excluído: ");
    scanf("%s", cpfAlvo);

    struct Cliente clienteAtual;

    // Lê o arquivo original e escreve no arquivo temporário sem o cliente a ser excluído
    while (fscanf(arqClientes, "%[^,],%d,%[^,],%[^,],%[^,],%[^\n]\n", clienteAtual.nome, &clienteAtual.idade,
                  clienteAtual.endereco, clienteAtual.profissao, clienteAtual.cpf, clienteAtual.rg) != EOF) {
        if (strcmp(clienteAtual.cpf, cpfAlvo) != 0) {
            // Cliente não é o alvo da exclusão, copia para o arquivo temporário
            fprintf(arqTemp, "%s,%d,%s,%s,%s,%s\n", clienteAtual.nome, clienteAtual.idade,
                    clienteAtual.endereco, clienteAtual.profissao, clienteAtual.cpf, clienteAtual.rg);
        }
    }

    fclose(arqClientes);
    fclose(arqTemp);

    remove("arquivos/clientes.txt");
    rename("arquivos/temp.txt", "arquivos/clientes.txt");

    printf("\nCliente excluído com sucesso!\n");
}

void consultarCliente() {
    FILE *arqClientes = fopen("arquivos/clientes.txt", "r");

    if (arqClientes == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    char cpfAlvo[15];
    printf("\nDigite o CPF do cliente a ser consultado: ");
    scanf("%s", cpfAlvo);

    struct Cliente clienteAtual;

    // Procura e exibe as informações do cliente com o CPF alvo
    while (fscanf(arqClientes, "%[^,],%d,%[^,],%[^,],%[^,],%[^\n]\n", clienteAtual.nome, &clienteAtual.idade,
                  clienteAtual.endereco, clienteAtual.profissao, clienteAtual.cpf, clienteAtual.rg) != EOF) {
        if (strcmp(clienteAtual.cpf, cpfAlvo) == 0) {
            // Cliente encontrado, exibe as informações
            printf("\nNome: %s\n", clienteAtual.nome);
            printf("Idade: %d\n", clienteAtual.idade);
            printf("Endereço: %s\n", clienteAtual.endereco);
            printf("Profissão: %s\n", clienteAtual.profissao);
            printf("CPF: %s\n", clienteAtual.cpf);
            printf("RG: %s\n", clienteAtual.rg);

            fclose(arqClientes);

            printf("\nConsulta de cliente realizada com sucesso!\n");
            return;
        }
    }

    fclose(arqClientes);

    printf("\nCliente não encontrado.\n");
}
