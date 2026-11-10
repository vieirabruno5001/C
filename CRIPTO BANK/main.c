
#include <stdio.h>  // Biblioteca para entrada e saida de dados
#include <stdlib.h> // Biblioteca para funcoes auxiliares
#include <string.h> // Biblioteca para trabalhar com textos

// Estrutura que guarda os dados do usuario
typedef struct {
    char nome[100];  // Nome do usuario
    char cpf[12];    // CPF do usuario
    char senha[7];   // Senha do usuario
    float reais;     // Saldo em reais
    float bitcoin;   // Quantidade de Bitcoin
    float ethereum;  // Quantidade de Ethereum
    float ripple;    // Quantidade de Ripple
} Carteira;

// Estrutura que guarda os precos das criptomoedas
typedef struct {
    float bitcoin;  // Preco do Bitcoin
    float ethereum; // Preco do Ethereum
    float ripple;   // Preco do Ripple
} Cotacao;

// Funcoes utilizadas no programa
void carregarCarteira(Carteira *c);
void salvarCarteira(Carteira c);
void carregarCotacoes(Cotacao *cot);
void salvarCotacoes(Cotacao cot);
void registrarExtrato(char *descricao);

void consultarSaldo(Carteira c);
void consultarExtrato(Carteira c);
void depositarReais(Carteira *c);
void sacarReais(Carteira *c);
void comprarCripto(Carteira *c, Cotacao cot);
void venderCripto(Carteira *c, Cotacao cot);
void atualizarCotacao(Cotacao *cot);

// Funcao principal do programa
int main() {

    Carteira usuario; // Guarda os dados do usuario
    Cotacao cotacao;  // Guarda os precos das moedas

    // Carrega os dados salvos nos arquivos
    carregarCarteira(&usuario);
    carregarCotacoes(&cotacao);

    char cpfInput[12];  // Guarda o CPF digitado
    char senhaInput[7]; // Guarda a senha digitada

    // Tela inicial
    printf("-----------------------------------------\n");
    printf("        BEM-VINDO AO CRIPTO BANK!  \n");
    printf("-----------------------------------------\n");

    // Pede o CPF e a senha para entrar
    printf("Digite o seu CPF (apenas numeros): ");
    scanf("%s", cpfInput);

    printf("Digite a sua senha (6 digitos): ");
    scanf("%s", senhaInput);

    // Verifica se o CPF e a senha estao corretos
    if (strcmp(cpfInput, usuario.cpf) != 0 ||
        strcmp(senhaInput, usuario.senha) != 0) {

        printf("\nCPF ou senha incorretos! Encerrando o programa...\n");
        return 0; // Encerra o programa
    }

    int opcao = 0; // Guarda a opcao escolhida

    // Repete o menu ate o usuario escolher sair
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

        // Recebe a opcao escolhida
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Executa a funcao escolhida
        switch (opcao) {

            case 1:
                consultarSaldo(usuario); // Consulta o saldo
                break;

            case 2:
                consultarExtrato(usuario); // Mostra o extrato
                break;

            case 3:
                depositarReais(&usuario); // Faz um deposito
                salvarCarteira(usuario); // Salva os dados
                break;

            case 4:
                sacarReais(&usuario); // Faz um saque
                salvarCarteira(usuario); // Salva os dados
                break;

            case 5:
                comprarCripto(&usuario, cotacao); // Compra moedas
                salvarCarteira(usuario); // Salva os dados
                break;

            case 6:
                venderCripto(&usuario, cotacao); // Vende moedas
                salvarCarteira(usuario); // Salva os dados
                break;

            case 7:
                atualizarCotacao(&cotacao); // Atualiza os precos
                salvarCotacoes(cotacao); // Salva os novos precos
                break;

            case 8:
                printf("\nObrigado por utilizar o Cripto Bank! Ate logo.\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    }

    return 0; // Finaliza o programa
}

// Carrega os dados do usuario
void carregarCarteira(Carteira *c) {

    FILE *arq = fopen("carteira.txt", "r"); // Abre o arquivo

    // Se o arquivo nao existir, cria uma carteira padrao
    if (arq == NULL) {

        strcpy(c->nome, "Bruno Vieira");
        strcpy(c->cpf, "12345678900");
        strcpy(c->senha, "123456");

        c->reais = 500.00;
        c->bitcoin = 0.0200;
        c->ethereum = 1.00;
        c->ripple = 200.00;

        salvarCarteira(*c); // Salva os dados

    } else {

        // Le os dados salvos no arquivo
        fscanf(arq, " %[^\n]\n", c->nome);
        fscanf(arq, "%s\n", c->cpf);
        fscanf(arq, "%s\n", c->senha);
        fscanf(arq, "%f\n", &c->reais);
        fscanf(arq, "%f\n", &c->bitcoin);
        fscanf(arq, "%f\n", &c->ethereum);
        fscanf(arq, "%f\n", &c->ripple);

        fclose(arq); // Fecha o arquivo
    }
}

// Salva os dados do usuario
void salvarCarteira(Carteira c) {

    FILE *arq = fopen("carteira.txt", "w"); // Abre o arquivo para salvar

    if (arq != NULL) {

        // Salva todas as informacoes da carteira
        fprintf(arq, "%s\n", c.nome);
        fprintf(arq, "%s\n", c.cpf);
        fprintf(arq, "%s\n", c.senha);
        fprintf(arq, "%.2f\n", c.reais);
        fprintf(arq, "%.4f\n", c.bitcoin);
        fprintf(arq, "%.4f\n", c.ethereum);
        fprintf(arq, "%.2f\n", c.ripple);

        fclose(arq); // Fecha o arquivo
    }
}

// Carrega os precos das criptomoedas
void carregarCotacoes(Cotacao *cot) {

    FILE *arq = fopen("cotacoes.txt", "r"); // Abre o arquivo

    // Se nao existir, coloca precos iniciais
    if (arq == NULL) {

        cot->bitcoin = 300000.00;
        cot->ethereum = 15000.00;
        cot->ripple = 3.00;

        salvarCotacoes(*cot); // Salva os precos

    } else {

        // Le os precos salvos
        fscanf(arq, "%f\n", &cot->bitcoin);
        fscanf(arq, "%f\n", &cot->ethereum);
        fscanf(arq, "%f\n", &cot->ripple);

        fclose(arq); // Fecha o arquivo
    }
}

// Salva os precos das moedas
void salvarCotacoes(Cotacao cot) {

    FILE *arq = fopen("cotacoes.txt", "w");

    if (arq != NULL) {

        fprintf(arq, "%.2f\n", cot.bitcoin);
        fprintf(arq, "%.2f\n", cot.ethereum);
        fprintf(arq, "%.2f\n", cot.ripple);

        fclose(arq); // Fecha o arquivo
    }
}

// Adiciona uma operacao ao extrato
void registrarExtrato(char *descricao) {

    FILE *arq = fopen("extrato.txt", "a"); // Abre para adicionar informacoes

    if (arq != NULL) {

        fprintf(arq, "%s\n", descricao); // Salva a operacao

        fclose(arq); // Fecha o arquivo
    }
}


 // 1. Consultar saldo
void consultarSaldo(Carteira c) {

    char senhaInput[7]; // Guarda a senha digitada

    printf("\n---- CONSULTAR SALDO ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    // Verifica se a senha esta correta
    if (strcmp(senhaInput, c.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    // Mostra os dados e saldos do usuario
    printf("\nNome: %s\n", c.nome);
    printf("CPF: %s\n", c.cpf);
    printf("Reais: R$ %.2f\n", c.reais);
    printf("Bitcoin: %.4f BTC\n", c.bitcoin);
    printf("Ethereum: %.4f ETH\n", c.ethereum);
    printf("Ripple: %.2f XRP\n", c.ripple);
}

// 2. Consultar extrato
void consultarExtrato(Carteira c) {

    char senhaInput[7]; // Guarda a senha digitada

    printf("\n---- CONSULTAR EXTRATO ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    // Verifica se a senha esta correta
    if (strcmp(senhaInput, c.senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    // Abre o arquivo do extrato
    FILE *arq = fopen("extrato.txt", "r");

    if (arq == NULL) {
        printf("\nNenhuma operacao registrada no extrato ainda.\n");
        return;
    }

    printf("\n---- EXTRATO DE OPERACOES ----\n");

    char linha[200]; // Guarda cada linha do extrato

    // Le e mostra todas as operacoes
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        printf("%s", linha);
    }

    fclose(arq); // Fecha o arquivo
}

// 3. Depositar reais
void depositarReais(Carteira *c) {

    float valor; // Valor que sera depositado

    printf("\n---- DEPOSITAR REAIS ----\n");
    printf("Digite o valor do deposito: R$ ");
    scanf("%f", &valor);

    // Verifica se o valor e valido
    if (valor <= 0) {
        printf("Valor invalido para deposito!\n");
        return;
    }

    c->reais += valor; // Adiciona o valor ao saldo

    printf("\nDeposito realizado com sucesso!\n");
    printf("Novo saldo em Reais: R$ %.2f\n", c->reais);

    char historico[150]; // Guarda a descricao do deposito

    // Monta a mensagem para o extrato
    sprintf(historico,
            "DEPOSITO: + R$ %.2f | Saldo Reais: R$ %.2f",
            valor, c->reais);

    registrarExtrato(historico); // Salva no extrato
}

// 4. Sacar reais
void sacarReais(Carteira *c) {

    float valor; // Valor que sera sacado

    printf("\n---- SACAR REAIS ----\n");
    printf("Digite o valor do saque: R$ ");
    scanf("%f", &valor);

    // Verifica se o valor e valido
    if (valor <= 0) {
        printf("Valor invalido para saque!\n");
        return;
    }

    // Verifica se existe saldo suficiente
    if (valor > c->reais) {
        printf("Saldo insuficiente! Seu saldo atual e de R$ %.2f\n", c->reais);
        return;
    }

    c->reais -= valor; // Retira o valor do saldo

    printf("\nSaque realizado com sucesso!\n");
    printf("Novo saldo em Reais: R$ %.2f\n", c->reais);

    char historico[150]; // Guarda a descricao do saque

    // Monta a mensagem do saque
    sprintf(historico,
            "SAQUE: - R$ %.2f | Saldo Reais: R$ %.2f",
            valor, c->reais);

    registrarExtrato(historico); // Salva no extrato
}

// 5. Comprar criptomoedas
void comprarCripto(Carteira *c, Cotacao cot) {

    char senhaInput[7]; // Guarda a senha digitada

    printf("\n---- COMPRAR CRIPTOMOEDAS ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    // Verifica a senha antes da compra
    if (strcmp(senhaInput, c->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    // Mostra os precos e as taxas das moedas
    printf("\nCotacoes Atuais:\n");
    printf("1. Bitcoin (BTC): R$ %.2f (Taxa de compra: 2%%)\n", cot.bitcoin);
    printf("2. Ethereum (ETH): R$ %.2f (Taxa de compra: 1%%)\n", cot.ethereum);
    printf("3. Ripple (XRP): R$ %.2f (Taxa de compra: 1%%)\n", cot.ripple);

    int opcao; // Guarda a moeda escolhida

    printf("Escolha a criptomoeda que deseja comprar (1, 2 ou 3): ");
    scanf("%d", &opcao);

    float valorGasto; // Valor usado na compra

    printf("Digite quanto em Reais (R$) voce deseja gastar: ");
    scanf("%f", &valorGasto);

    // Verifica se o valor e valido e se ha saldo suficiente
    if (valorGasto <= 0 || valorGasto > c->reais) {
        printf("Valor invalido ou saldo em Reais insuficiente!\n");
        return;
    }

    float taxa = 0; // Valor da taxa
    float valorLiquido = 0; // Valor depois da taxa
    float qtdComprada = 0; // Quantidade comprada

    char historico[150]; // Guarda a descricao da compra

    // Compra de Bitcoin
    if (opcao == 1) {

        taxa = valorGasto * 0.02; // Calcula 2% de taxa
        valorLiquido = valorGasto - taxa; // Retira a taxa
        qtdComprada = valorLiquido / cot.bitcoin; // Calcula a quantidade

        c->reais -= valorGasto; // Retira o dinheiro
        c->bitcoin += qtdComprada; // Adiciona os Bitcoins

        printf("\nCompra de Bitcoin realizada!\n");
        printf("Quantidade comprada: %.6f BTC\n", qtdComprada);

        sprintf(historico,
                "COMPRA BTC: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.6f BTC",
                valorGasto, taxa, qtdComprada);

    // Compra de Ethereum
    } else if (opcao == 2) {

        taxa = valorGasto * 0.01; // Calcula 1% de taxa
        valorLiquido = valorGasto - taxa;
        qtdComprada = valorLiquido / cot.ethereum;

        c->reais -= valorGasto;
        c->ethereum += qtdComprada;

        printf("\nCompra de Ethereum realizada!\n");
        printf("Quantidade comprada: %.6f ETH\n", qtdComprada);

        sprintf(historico,
                "COMPRA ETH: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.6f ETH",
                valorGasto, taxa, qtdComprada);

    // Compra de Ripple
    } else if (opcao == 3) {

        taxa = valorGasto * 0.01; // Calcula 1% de taxa
        valorLiquido = valorGasto - taxa;
        qtdComprada = valorLiquido / cot.ripple;

        c->reais -= valorGasto;
        c->ripple += qtdComprada;

        printf("\nCompra de Ripple realizada!\n");
        printf("Quantidade comprada: %.2f XRP\n", qtdComprada);

        sprintf(historico,
                "COMPRA XRP: Gastou R$ %.2f (Taxa: R$ %.2f) | Comprou: %.2f XRP",
                valorGasto, taxa, qtdComprada);

    } else {
        // Mostra mensagem se a moeda nao for valida
        printf("Opcao de criptomoeda invalida!\n");
        return;
    }

    registrarExtrato(historico); // Salva a compra no extrato
}

// 6. Vender criptomoedas
void venderCripto(Carteira *c, Cotacao cot) {

    char senhaInput[7]; // Guarda a senha digitada

    printf("\n---- VENDER CRIPTOMOEDAS ----\n");
    printf("Digite sua senha para confirmar: ");
    scanf("%s", senhaInput);

    // Verifica se a senha esta correta
    if (strcmp(senhaInput, c->senha) != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    // Mostra os saldos e precos das moedas
    printf("\nSeus saldos de criptomoedas:\n");
    printf("1. Bitcoin: %.6f BTC (Cotacao: R$ %.2f | Taxa de venda: 3%%)\n",
           c->bitcoin, cot.bitcoin);

    printf("2. Ethereum: %.6f ETH (Cotacao: R$ %.2f | Taxa de venda: 2%%)\n",
           c->ethereum, cot.ethereum);

    printf("3. Ripple: %.2f XRP (Cotacao: R$ %.2f | Taxa de venda: 1%%)\n",
           c->ripple, cot.ripple);

    int opcao; // Guarda a moeda escolhida

    printf("Escolha a criptomoeda que deseja vender (1, 2 ou 3): ");
    scanf("%d", &opcao);

    float qtdVender; // Quantidade que sera vendida

    printf("Digite a quantidade que deseja vender: ");
    scanf("%f", &qtdVender);

    // Verifica se a quantidade e valida
    if (qtdVender <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    float valorBruto = 0; // Valor antes da taxa
    float taxa = 0; // Valor da taxa
    float valorRecebido = 0; // Valor recebido depois da taxa

    char historico[150]; // Guarda a descricao da venda

    // Venda de Bitcoin
    if (opcao == 1) {

        // Verifica se ha Bitcoin suficiente
        if (qtdVender > c->bitcoin) {
            printf("Saldo de Bitcoin insuficiente!\n");
            return;
        }

        valorBruto = qtdVender * cot.bitcoin; // Calcula o valor da venda
        taxa = valorBruto * 0.03; // Calcula 3% de taxa
        valorRecebido = valorBruto - taxa; // Desconta a taxa

        c->bitcoin -= qtdVender; // Retira os Bitcoins
        c->reais += valorRecebido; // Adiciona o dinheiro

        printf("\nVenda de Bitcoin realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);

        sprintf(historico,
                "VENDA BTC: Vendeu %.6f BTC | Recebeu R$ %.2f (Taxa: R$ %.2f)",
                qtdVender, valorRecebido, taxa);

    // Venda de Ethereum
    } else if (opcao == 2) {

        // Verifica se ha Ethereum suficiente
        if (qtdVender > c->ethereum) {
            printf("Saldo de Ethereum insuficiente!\n");
            return;
        }

        valorBruto = qtdVender * cot.ethereum;
        taxa = valorBruto * 0.02; // Calcula 2% de taxa
        valorRecebido = valorBruto - taxa;

        c->ethereum -= qtdVender;
        c->reais += valorRecebido;

        printf("\nVenda de Ethereum realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);

        sprintf(historico,
                "VENDA ETH: Vendeu %.6f ETH | Recebeu R$ %.2f (Taxa: R$ %.2f)",
                qtdVender, valorRecebido, taxa);

    // Venda de Ripple
    } else if (opcao == 3) {

        // Verifica se ha Ripple suficiente
        if (qtdVender > c->ripple) {
            printf("Saldo de Ripple insuficiente!\n");
            return;
        }

        valorBruto = qtdVender * cot.ripple;
        taxa = valorBruto * 0.01; // Calcula 1% de taxa
        valorRecebido = valorBruto - taxa;

        c->ripple -= qtdVender;
        c->reais += valorRecebido;

        printf("\nVenda de Ripple realizada!\n");
        printf("Valor creditado em Reais: R$ %.2f\n", valorRecebido);

        sprintf(historico,
                "VENDA XRP: Vendeu %.2f XRP | Recebeu R$ %.2f (Taxa: R$ %.2f)",
                qtdVender, valorRecebido, taxa);

    } else {
        // Mostra mensagem se a opcao nao for valida
        printf("Opcao de criptomoeda invalida!\n");
        return;
    }

    registrarExtrato(historico); // Salva a venda no extrato
}

// 7. Atualizar cotacao
void atualizarCotacao(Cotacao *cot) {

    printf("\n---- ATUALIZAR COTACAO ---- \n");

    // Cria uma variacao aleatoria entre -5% e +5%
    float variacaoBTC = ((rand() % 11) - 5) / 100.0;
    float variacaoETH = ((rand() % 11) - 5) / 100.0;
    float variacaoXRP = ((rand() % 11) - 5) / 100.0;

    // Atualiza o preco de cada moeda
    cot->bitcoin += cot->bitcoin * variacaoBTC;
    cot->ethereum += cot->ethereum * variacaoETH;
    cot->ripple += cot->ripple * variacaoXRP;

    printf("Cotacoes atualizadas com sucesso (variacao entre -5%% e +5%%):\n");

    // Mostra os novos precos
    printf("Novo valor Bitcoin: R$ %.2f\n", cot->bitcoin);
    printf("Novo valor Ethereum: R$ %.2f\n", cot->ethereum);
    printf("Novo valor Ripple: R$ %.2f\n", cot->ripple);
}
