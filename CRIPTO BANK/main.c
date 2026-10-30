#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar a carteira do investidor
typedef struct {
    char nome[100];
    char cpf[12];
    char senha[7];
    float reais;
    float bitcoin;
    float ethereum;
    float ripple;
} Carteira;

// Estrutura para armazenar as cotacoes
typedef struct {
    float bitcoin;
    float ethereum;
    float ripple;
} Cotacao;

// Protótipos das funções
void carregarCarteira(Carteira *c);
void salvarCarteira(Carteira c);
void carregarCotacoes(Cotacao *cot);
void salvarCotacoes(Cotacao cot);
void registrarExtrato(char *descricao);

void consultarSaldo(Carteira c);
void consultarExtrato(Carteira c); // <--- Atualizado para receber a carteira e validar a senha
void depositarReais(Carteira *c);
void sacarReais(Carteira *c);
void comprarCripto(Carteira *c, Cotacao cot);
void venderCripto(Carteira *c, Cotacao cot);
void atualizarCotacao(Cotacao *cot);

int main() {
    Carteira usuario;
    Cotacao cotacao;

    // Carrega os dados gravados nos arquivos
    carregarCarteira(&usuario);
    carregarCotacoes(&cotacao);

    char cpfInput[12];
    char senhaInput[7];

    printf("-----------------------------------------\n");
    printf("        BEM-VINDO AO CRIPTO BANK!  \n");
    printf("-----------------------------------------\n");

    // Tela de Login
    printf("Digite o seu CPF (apenas numeros): ");
    scanf("%s", cpfInput);
    printf("Digite a sua senha (6 digitos): ");
    scanf("%s", senhaInput);

    // Validação de Login simples
    if (strcmp(cpfInput, usuario.cpf) != 0 || strcmp(senhaInput, usuario.senha) != 0) {
        printf("\nCPF ou senha incorretos! Encerrando o programa...\n");
        return 0;
    }

    int opcao = 0;

    // Loop do Menu Principal
    while (opcao != 8) {
        printf("\n-----------------------------------------\n");
        printf("            MENU CRIPTO BANK             \n");
        printf("-----------------------------------------\n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar reais\n");
        printf("4. Sacar reais\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Atualizar cotacao das criptomoedas\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                consultarSaldo(usuario);
                break;
            case 2:
                consultarExtrato(usuario); // <--- Passando o usuário para validar a senha
                break;
            case 3:
                depositarReais(&usuario);
                salvarCarteira(usuario);
                break;
            case 4:
                sacarReais(&usuario);
                salvarCarteira(usuario);
                break;
            case 5:
                comprarCripto(&usuario, cotacao);
                salvarCarteira(usuario);
                break;
            case 6:
                venderCripto(&usuario, cotacao);
                salvarCarteira(usuario);
                break;
            case 7:
                atualizarCotacao(&cotacao);
                salvarCotacoes(cotacao);
                break;
            case 8:
                printf("\nObrigado por utilizar o Cripto Bank! Ate logo.\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    }

    return 0;
}

// Carrega os dados da carteira a partir de arquivo TXT
void carregarCarteira(Carteira *c) {
    FILE *arq = fopen("carteira.txt", "r");

    if (arq == NULL) {
        // Se o arquivo nao existir, cria com dados padrao para teste
        strcpy(c->nome, "Bruno Vieira");
        strcpy(c->cpf, "12345678900");
        strcpy(c->senha, "123456");
        c->reais = 500.00;
        c->bitcoin = 0.0200;
        c->ethereum = 1.00;
        c->ripple = 200.00;
        salvarCarteira(*c);
    } else {
        fscanf(arq, " %[^\n]\n", c->nome);
        fscanf(arq, "%s\n", c->cpf);
        fscanf(arq, "%s\n", c->senha);
        fscanf(arq, "%f\n", &c->reais);
        fscanf(arq, "%f\n", &c->bitcoin);
        fscanf(arq, "%f\n", &c->ethereum);
        fscanf(arq, "%f\n", &c->ripple);
        fclose(arq);
    }
}

// Salva os dados atualizados da carteira no TXT
void salvarCarteira(Carteira c) {
    FILE *arq = fopen("carteira.txt", "w");
    if (arq != NULL) {
        fprintf(arq, "%s\n", c.nome);
        fprintf(arq, "%s\n", c.cpf);
        fprintf(arq, "%s\n", c.senha);
        fprintf(arq, "%.2f\n", c.reais);
        fprintf(arq, "%.4f\n", c.bitcoin);
        fprintf(arq, "%.4f\n", c.ethereum);
        fprintf(arq, "%.2f\n", c.ripple);
        fclose(arq);
    }
}

// Carrega as cotacoes a partir de arquivo TXT
void carregarCotacoes(Cotacao *cot) {
    FILE *arq = fopen("cotacoes.txt", "r");

    if (arq == NULL) {
        // Cotacoes iniciais padrao
        cot->bitcoin = 300000.00;
        cot->ethereum = 15000.00;
        cot->ripple = 3.00;
        salvarCotacoes(*cot);
    } else {
        fscanf(arq, "%f\n", &cot->bitcoin);
        fscanf(arq, "%f\n", &cot->ethereum);
        fscanf(arq, "%f\n", &cot->ripple);
        fclose(arq);
    }
}

// Salva as cotacoes atualizadas no TXT
void salvarCotacoes(Cotacao cot) {
    FILE *arq = fopen("cotacoes.txt", "w");
    if (arq != NULL) {
        fprintf(arq, "%.2f\n", cot.bitcoin);
        fprintf(arq, "%.2f\n", cot.ethereum);
        fprintf(arq, "%.2f\n", cot.ripple);
        fclose(arq);
    }
}

// Adiciona uma linha de texto ao arquivo de extrato
void registrarExtrato(char *descricao) {
    FILE *arq = fopen("extrato.txt", "a");
    if (arq != NULL) {
        fprintf(arq, "%s\n", descricao);
        fclose(arq);
    }
}

// 1. Consultar Saldo
void consultarSaldo(Carteira c) {
    char senhaInput[7];
    printf("\n---- CONSULTAR SALDO ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    if (strcmp(senhaInput, c.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\nNome: %s\n", c.nome);
    printf("CPF: %s\n", c.cpf);
    printf("Reais: R$ %.2f\n", c.reais);
    printf("Bitcoin: %.4f BTC\n", c.bitcoin);
    printf("Ethereum: %.4f ETH\n", c.ethereum);
    printf("Ripple: %.2f XRP\n", c.ripple);
}

// 2. Consultar Extrato (Com validação de senha)
void consultarExtrato(Carteira c) {
    char senhaInput[7];
    printf("\n---- CONSULTAR EXTRATO ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    if (strcmp(senhaInput, c.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    FILE *arq = fopen("extrato.txt", "r");
    if (arq == NULL) {
        printf("\nNenhuma operacao registrada no extrato ainda.\n");
        return;
    }

    printf("\n---- EXTRATO DE OPERACOES ----\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        printf("%s", linha);
    }
    fclose(arq);
}

// 3. Depositar Reais
void depositarReais(Carteira *c) {
    float valor;
    printf("\n---- DEPOSITAR REAIS ----\n");
    printf("Digite o valor do deposito: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido para deposito!\n");
        return;
    }

    c->reais += valor;
    printf("\nDeposito realizado com sucesso!\n");
    printf("Novo saldo em Reais: R$ %.2f\n", c->reais);

    char historico[150];
    sprintf(historico, "DEPOSITO: + R$ %.2f | Saldo Reais: R$ %.2f", valor, c->reais);
    registrarExtrato(historico);
}

// 4. Sacar Reais
void sacarReais(Carteira *c) {
    float valor;
    printf("\n---- SACAR REAIS ----\n");
    printf("Digite o valor do saque: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor invalido para saque!\n");
        return;
    }

    if (valor > c->reais) {
        printf("Saldo insuficiente! Seu saldo atual e de R$ %.2f\n", c->reais);
        return;
    }

    c->reais -= valor;
    printf("\nSaque realizado com sucesso!\n");
    printf("Novo saldo em Reais: R$ %.2f\n", c->reais);

    char historico[150];
    sprintf(historico, "SAQUE: - R$ %.2f | Saldo Reais: R$ %.2f", valor, c->reais);
    registrarExtrato(historico);
}

// 5. Comprar Criptomoedas
void comprarCripto(Carteira *c, Cotacao cot) {
    char senhaInput[7];
    printf("\n---- COMPRAR CRIPTOMOEDAS ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    if (strcmp(senhaInput, c->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\nCotacoes Atuais:\n");
    printf("1. Bitcoin (BTC): R$ %.2f (Taxa de compra: 2%%)\n", cot.bitcoin);
    printf("2. Ethereum (ETH): R$ %.2f (Taxa de compra: 1%%)\n", cot.ethereum);
    printf("3. Ripple (XRP): R$ %.2f (Taxa de compra: 1%%)\n", cot.ripple);

    int opcao;
    printf("Escolha a criptomoeda que deseja comprar (1, 2 ou 3): ");
    scanf("%d", &opcao);

    float valorGasto;
    printf("Digite quanto em Reais (R$) voce deseja gastar: ");
    scanf("%f", &valorGasto);

    if (valorGasto <= 0 || valorGasto > c->reais) {
        printf("Valor invalido ou saldo em Reais insuficiente!\n");
        return;
    }

    float taxa = 0;
    float valorLiquido = 0;
    float qtdComprada = 0;
    char historico[150];

    if (opcao == 1) { // Bitcoin
        taxa = valorGasto * 0.02; 
        valorLiquido = valorGasto - taxa;
        qtdComprada = valorLiquido / cot.bitcoin;
        c->reais -= valorGasto;
        c->bitcoin += qtdComprada;

        printf("\nCompra de Bitcoin realizada!\n");
        printf("Quantidade comprada: %.6f BTC\n", qtdComprada);
        sprintf(historico, "COMPRA BTC: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.6f BTC", valorGasto, taxa, qtdComprada);

    } else if (opcao == 2) { // Ethereum
        taxa = valorGasto * 0.01; 
        valorLiquido = valorGasto - taxa;
        qtdComprada = valorLiquido / cot.ethereum;
        c->reais -= valorGasto;
        c->ethereum += qtdComprada;

        printf("\nCompra de Ethereum realizada!\n");
        printf("Quantidade comprada: %.6f ETH\n", qtdComprada);
        sprintf(historico, "COMPRA ETH: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.6f ETH", valorGasto, taxa, qtdComprada);

    } else if (opcao == 3) { // Ripple
        taxa = valorGasto * 0.01; 
        valorLiquido = valorGasto - taxa;
        qtdComprada = valorLiquido / cot.ripple;
        c->reais -= valorGasto;
        c->ripple += qtdComprada;

        printf("\nCompra de Ripple realizada!\n");
        printf("Quantidade comprada: %.2f XRP\n", qtdComprada);
        sprintf(historico, "COMPRA XRP: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.2f XRP", valorGasto, taxa, qtdComprada);

    } else {
        printf("Opcao de criptomoeda invalida!\n");
        return;
    }

    registrarExtrato(historico);
}

// 6. Vender Criptomoedas
void venderCripto(Carteira *c, Cotacao cot) {
    char senhaInput[7];
    printf("\n---- VENDER CRIPTOMOEDAS ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    if (strcmp(senhaInput, c->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    printf("\nSeus saldos de criptomoedas:\n");
    printf("1. Bitcoin: %.6f BTC (Cotacao: R$ %.2f | Taxa de venda: 3%%)\n", c->bitcoin, cot.bitcoin);
    printf("2. Ethereum: %.6f ETH (Cotacao: R$ %.2f | Taxa de venda: 2%%)\n", c->ethereum, cot.ethereum);
    printf("3. Ripple: %.2f XRP (Cotacao: R$ %.2f | Taxa de venda: 1%%)\n", c->ripple, cot.ripple);

    int opcao;
    printf("Escolha a criptomoeda que deseja vender (1, 2 ou 3): ");
    scanf("%d", &opcao);

    float qtdVender;
    printf("Digite a quantidade que deseja vender: ");
    scanf("%f", &qtdVender);

    if (qtdVender <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    float valorBruto = 0;
    float taxa = 0;
    float valorRecebido = 0;
    char historico[150];

    if (opcao == 1) { // Bitcoin
        if (qtdVender > c->bitcoin) {
            printf("Saldo de Bitcoin insuficiente!\n");
            return;
        }
        valorBruto = qtdVender * cot.bitcoin;
        taxa = valorBruto * 0.03; // 3%
        valorRecebido = valorBruto - taxa;

        c->bitcoin -= qtdVender;
        c->reais += valorRecebido;

        printf("\nVenda de Bitcoin realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);
        sprintf(historico, "VENDA BTC: Vendeu %.6f BTC | Recebeu R$ %.2f (Taxa: R$ %.2f)", qtdVender, valorRecebido, taxa);

    } else if (opcao == 2) { // Ethereum
        if (qtdVender > c->ethereum) {
            printf("Saldo de Ethereum insuficiente!\n");
            return;
        }
        valorBruto = qtdVender * cot.ethereum;
        taxa = valorBruto * 0.02; // 2%
        valorRecebido = valorBruto - taxa;

        c->ethereum -= qtdVender;
        c->reais += valorRecebido;

        printf("\nVenda de Ethereum realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);
        sprintf(historico, "VENDA ETH: Vendeu %.6f ETH | Recebeu R$ %.2f (Taxa: R$ %.2f)", qtdVender, valorRecebido, taxa);

    } else if (opcao == 3) { // Ripple
        if (qtdVender > c->ripple) {
            printf("Saldo de Ripple insuficiente!\n");
            return;
        }
        valorBruto = qtdVender * cot.ripple;
        taxa = valorBruto * 0.01; 
        valorRecebido = valorBruto - taxa;

        c->ripple -= qtdVender;
        c->reais += valorRecebido;

        printf("\nVenda de Ripple realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);
        sprintf(historico, "VENDA XRP: Vendeu %.2f XRP | Recebeu R$ %.2f (Taxa: R$ %.2f)", qtdVender, valorRecebido, taxa);

    } else {
        printf("Opcao de criptomoeda invalida!\n");
        return;
    }

    registrarExtrato(historico);
}

// 7. Atualizar Cotação
void atualizarCotacao(Cotacao *cot) {
    printf("\n---- ATUALIZAR COTACAO ---- \n");

    // Sorteia um número de -5 a +5 para cada moeda
    float variacaoBTC = ((rand() % 11) - 5) / 100.0; // Ex: -0.05 ate +0.05
    float variacaoETH = ((rand() % 11) - 5) / 100.0;
    float variacaoXRP = ((rand() % 11) - 5) / 100.0;

    cot->bitcoin += cot->bitcoin * variacaoBTC;
    cot->ethereum += cot->ethereum * variacaoETH;
    cot->ripple += cot->ripple * variacaoXRP;

    printf("Cotacoes atualizadas com sucesso (variacao entre -5%% e +5%%):\n");
    printf("Novo valor Bitcoin: R$ %.2f\n", cot->bitcoin);
    printf("Novo valor Ethereum: R$ %.2f\n", cot->ethereum);
    printf("Novo valor Ripple: R$ %.2f\n", cot->ripple);
}