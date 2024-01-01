#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Assento{
    int reservado;
    char fileira;
    int numero;
    char nome[100];
} Assento;

typedef struct Teatro{
    Assento **assentos;
    int fileira;
    int numero;
} Teatro;

// ----------FUNÇÕES PARA CONVERTER INTEIROS E LETRAS----------

// converte inteiros em letras
char converterIntParaChar(int n){
    return (char)('A' + n);
}

// converte letras em inteiros
int converterCharParaInt(char c){
    if('A'<=c && c<='Z'){
        return c - 'A' + 1;
    } else {
        return 0;
    }
}

// ----------FUNÇÕES RELATIVAS A TEATRO E ASSENTO----------

// função do tipo assento para inicializar a matriz
Assento **inicializarMatriz(int numLinhas, int numColunas){
    Assento **matriz = (Assento **) malloc(sizeof(struct Assento *)*numLinhas);
    for(int i = 0; i < numLinhas; i++) {
        matriz[i] = (Assento *)malloc(sizeof(struct Assento)*numColunas);
    }
    for (int i = 0; i < numLinhas; i++){
        for (int j = 0; j < numColunas; j++){
            matriz[i][j].reservado = 0;
            matriz[i][j].fileira = converterIntParaChar(i);
            matriz[i][j].numero = j + 1;
            matriz[i][j].nome[0] = '\0';
        }
    }
    return matriz;
}

// função do tipo teatro para cirar o teatro
Teatro criarTeatro(int numLinhas, int numColunas){
    Teatro teatro;
    teatro.fileira = numLinhas;
    teatro.numero = numColunas;
    teatro.assentos = inicializarMatriz(numLinhas, numColunas);
    return teatro;
}

// função para exibir o teatro atual
void exibirTeatro(Teatro teatro){
    for (int i = 0; i <= teatro.numero; i++){
        if (i == 0){
            printf("  ");
        }
        else if (i < 10){
            printf(" %d  ", i);
        }
        else{
            printf(" %d ", i);
        }
    }
    printf("\n");
    for (int i = 0; i < teatro.fileira; i++){
        printf("%c ", teatro.assentos[i][0].fileira);
        for (int j = 0; j < teatro.numero; j++){
            if (teatro.assentos[i][j].reservado){
                printf("[X] ");
            }
            else{
                printf("[ ] ");
            }
        }
        printf("\n");
    }
}

// função para liberar a memória da matriz
void liberarMatriz(Assento **matriz, int numLinhas){
    for (int i = 0; i < numLinhas; i++){
        free(matriz[i]);
    }
    free(matriz);
}

// ----------FUNÇÕES RELATIVAS A RESERVAS E CANCELAMENTOS----------

// função para reservar um assento específico
int reservarAssento(Teatro t, int linha, int coluna, char *nome){
    if (t.assentos[linha][coluna].reservado == 0){
        t.assentos[linha][coluna].reservado = 1;
        strcpy(t.assentos[linha][coluna].nome, nome);
        char linhaChar = converterIntParaChar(linha);
        printf("Seu assento em %c%d foi reservado com sucesso!\n", linhaChar, coluna+1);
        return 1;
    } else {
        printf("Este assento está ocupado. Por favor, escolha outro.\n");
        return 0;
    }
}

// função para reservar assentos consecutivos específicos
int reservarAssentosConsecutivos(Teatro t, int linha, int coluna, int n, char *nome){
    int consecutivosLivres = 0;
    for (int j = coluna; j <= coluna+(n-1); j++){
        if (t.assentos[linha][j].reservado == 0){
            consecutivosLivres++;
        }
    }
    if (consecutivosLivres>=n){
        for (int j = coluna; j <= coluna+(n-1); j++){
            if (t.assentos[linha][j].reservado == 0){
                t.assentos[linha][j].reservado = 1;
                strcpy(t.assentos[linha][j].nome, nome);
            }
        }
        return 1;
    } else {
        return 0;
    }
}

// função para reservar um assento pelo sistema
Assento * reservarAssentoPeloSistema(Teatro t, char *nome){
    for (int i = 0; i < t.fileira; i++) {
        for (int j = 0; j < t.numero; j++) {
            if (t.assentos[i][j].reservado == 0){
                t.assentos[i][j].reservado = 1;
                strcpy(t.assentos[i][j].nome, nome);
                return &t.assentos[i][j];
            }
        }
    }
    return NULL;
}

// função para reservar assentos consecutivos pelo sistema
Assento ** reservarAssentosConsecutivosPeloSistema(Teatro t, int n, char *nome){
    int consecutivosLivres = 0;
    Assento **assentosConsecutivos = NULL;
    for (int i = 0; i < t.fileira; i++) {
        for (int j = 0; j < t.numero; j++) {
            if (t.assentos[i][j].reservado == 0){
                consecutivosLivres++;
                if (consecutivosLivres==n){
                    assentosConsecutivos = (Assento **)malloc(n * sizeof(Assento *));
                    for (int k = 0; k < n; k++){
                        t.assentos[i][k+(j-n+1)].reservado = 1;
                        strcpy(t.assentos[i][k+(j-n+1)].nome, nome);
                        assentosConsecutivos[k] = &t.assentos[i][k+(j-n+1)];
                    }
                    return assentosConsecutivos;
                }
            } else {
                consecutivosLivres = 0;
            }
        }
        consecutivosLivres = 0;
    }
    return NULL;
}

// função para cancelar uma reserva específica
int cancelarReserva(Teatro t, int linha, int coluna){
    if (t.assentos[linha][coluna].reservado == 1){
        t.assentos[linha][coluna].reservado = 0;
        t.assentos[linha][coluna].nome[0] = '\0';
        return 1;
    } else {
        return 0;
    }
}

// função para cancelar todas as reservas de uma pessoa
int cancelarReservas(Teatro t, char *nome){
    int cancelamentos = 0;
    for (int i = 0; i < t.fileira; i++){
        for (int j = 0; j < t.numero; j++){
            if (t.assentos[i][j].reservado == 1 && strcmp(t.assentos[i][j].nome, nome) == 0){
                t.assentos[i][j].reservado = 0;
                t.assentos[i][j].nome[0] = '\0';
                cancelamentos++;
            }
        }
    }
    return cancelamentos;
}

// função para cancelar todas as reservas do teatro
int cancelarTodasAsReservas(Teatro t){
    int cancelamentos = 0;
    for (int i = 0; i < t.fileira; i++){
        for (int j = 0; j < t.numero; j++){
            if (t.assentos[i][j].reservado == 1){
                t.assentos[i][j].reservado = 0;
                t.assentos[i][j].nome[0] = '\0';
                cancelamentos++;
            }
        }
    }
    return cancelamentos;
}

// ----------FUNÇÕES RELATIVAS A CONSULTAS E ESTATÍSTICAS----------

// função para verificar disponibilidade do assento
int verificarDisponibilidade(Teatro t, int linha, int coluna){
    if(t.assentos[linha][coluna].reservado==1){
        return 0;
    } else{
        return 1;
    }
}

// função para verificar disponibilidade do assentos consecutivos
void consultarAssentosConsecutivos(Teatro t, int n){
    int assentosConsecutivos = 0, maiorSequenciaLivre = 0;
    char linha;
    for (int i = 0; i < t.fileira; i++){
        for (int j = 0; j < t.numero; j++){
            if (t.assentos[i][j].reservado == 1){
                assentosConsecutivos = 0;
            } else {
                assentosConsecutivos++;
                if (assentosConsecutivos>maiorSequenciaLivre){
                    maiorSequenciaLivre = assentosConsecutivos;
                }
            }
        }
        if (maiorSequenciaLivre>=n){
            linha = converterIntParaChar(i);
            printf("%d assentos consecutivos livres na linha %c\n", maiorSequenciaLivre, linha);
        }
        assentosConsecutivos = 0;
        maiorSequenciaLivre = 0;
    }
}

// função para exibir informações do teatro
void exibirInformacoes(Teatro t){
    int assentosLivres = 0, assentosOcupados = 0, maiorSequenciaLivre = 0, msl = 0;
    char linha;
    for (int i = 0; i < t.fileira; i++){
        for (int j = 0; j < t.numero; j++){
            if (t.assentos[i][j].reservado == 1){
                assentosOcupados++;
                maiorSequenciaLivre = 0;
            } else {
                assentosLivres++;
                maiorSequenciaLivre++;
                if (maiorSequenciaLivre>msl){
                    msl = maiorSequenciaLivre;
                    linha = converterIntParaChar(i);
                }
            }
        }
        maiorSequenciaLivre = 0;
    }
    printf("Assentos livres: %d\n", assentosLivres);
    printf("Assentos ocupados: %d\n", assentosOcupados);
    printf("Maior sequencia livre: %d assentos na linha %c\n", msl, linha);
}

// ----------FUNÇÕES RELATIVAS A ARMAZENAMENTO----------

// função para salvar o teatro em um arquivo
int salvarEstado(Teatro t, char *nomeArquivo){
    FILE *arq = fopen(nomeArquivo, "w");
    if (arq==NULL){
        return 0;
    }
    fprintf(arq, "%d %d\n", t.fileira, t.numero);
    for (int i = 0; i < t.fileira; i++) {
        for (int j = 0; j < t.numero; j++) {
            if (t.assentos[i][j].nome[0] == '\0') {
                char nome[] = "N/A";
                fprintf(arq, "%d\n", t.assentos[i][j].reservado);
                fprintf(arq, "%s\n", nome);
                fprintf(arq, "%c\n", t.assentos[i][j].fileira);
                fprintf(arq, "%d\n", t.assentos[i][j].numero);
            } else {
                fprintf(arq, "%d\n", t.assentos[i][j].reservado);
                fprintf(arq, "%s\n", t.assentos[i][j].nome);
                fprintf(arq, "%c\n", t.assentos[i][j].fileira);
                fprintf(arq, "%d\n", t.assentos[i][j].numero);
            }
        }
    }
    fclose(arq);
    return 1;
}

// função para carregar um teatro de um arquivo
Teatro carregarEstado(char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao carregar o arquivo %s\n", nomeArquivo);
        exit(EXIT_FAILURE);
    }
    int linha, coluna;
    fscanf(arq, "%d %d\n", &linha, &coluna);
    Teatro teatro = criarTeatro(linha, coluna);
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            fscanf(arq, "%d\n", &teatro.assentos[i][j].reservado);
            fscanf(arq, "%[^\n]\n", teatro.assentos[i][j].nome);
            fscanf(arq, "%c\n", &teatro.assentos[i][j].fileira);
            fscanf(arq, "%d\n", &teatro.assentos[i][j].numero);
        }
    }
    fclose(arq);
    printf("Estado do programa carregado com sucesso.\n");
    return teatro;
}

// ----------OUTRAS FUNÇÕES----------

// função para verififar se a linha digitada é válida
int verificarLinhaValida(Teatro t, char linha){
    if (converterCharParaInt(linha)==0 || converterCharParaInt(linha)>t.fileira){
        printf("Erro na entrada. Por favor, repita a operacao.\n");
        return 0;
    } else {
        return 1;
    }
}

// função para verififar se a coluna digitada é válida
int verificarColunaValida(Teatro t, int coluna){
    if (coluna<1 || coluna>t.numero){
        printf("Erro na entrada. Por favor, repita a operacao.\n");
        return 0;
    } else {
        return 1;
    }
}

// função para mostrar a lista de operações
void mostrarOperacoes(){
    printf(
        "============| Operacoes |============\n"
        "0 - Sair do programa\n"
        "1 - Salvar estado\n"
        "2 - Carregar estado\n"
        "3 - Visualizar\n"
        "4 - Reservar assento especifico\n"
        "5 - Reservar assentos consecutivos especificos\n"
        "6 - Reservar assento pelo sistema\n"
        "7 - Reservar assentos consecutivos pelo sistema\n"
        "8 - Cancelar reserva de um assento\n"
        "9 - Cancelar reservas de uma pessoa\n"
        "10- Cancelar todas as reservas\n"
        "11- Verificar disponibilidade\n"
        "12- Consultar assentos consecutivos\n"
        "13- Exibir informacoes\n");
}

int main(){
    printf("Bem-vindo(a) ao nosso Sistema de Reservas de Assentos!\n");
    int erro = 1, numLinhas, numColunas;
    while (erro==1){
        printf("Digite o numero de linhas do teatro (maximo 26): ");
        scanf("%d", &numLinhas);
        printf("Digite o numero de colunas do teatro (maximo 99): ");
        scanf("%d", &numColunas);
        if (numLinhas<27 && numColunas<100){
            erro=0;
        } else {
            printf("Entrada invalida, digite os valores novamente.\n");
        }
    }
    Teatro teatro = criarTeatro(numLinhas, numColunas);
    printf("Teatro criado comsucesso!\n\n");

    // ENTRADAS PARA TESTES
    // char nome1[] = "sebastiao fellipe";
    // char nome2[] = "fellipe sebastiao";
    // teatro.assentos[0][1].reservado = 1;
    // snprintf(teatro.assentos[0][1].nome, sizeof(teatro.assentos[0][1].nome), nome1);
    // teatro.assentos[1][2].reservado = 1;
    // snprintf(teatro.assentos[1][2].nome, sizeof(teatro.assentos[1][2].nome), nome1);
    // teatro.assentos[0][0].reservado = 1;
    // snprintf(teatro.assentos[0][0].nome, sizeof(teatro.assentos[0][0].nome), nome1);
    // teatro.assentos[2][4].reservado = 1;
    // snprintf(teatro.assentos[2][4].nome, sizeof(teatro.assentos[2][4].nome), nome1);
    // teatro.assentos[3][1].reservado = 1;
    // snprintf(teatro.assentos[3][1].nome, sizeof(teatro.assentos[3][1].nome), nome2);  
    // teatro.assentos[3][2].reservado = 1;
    // snprintf(teatro.assentos[3][2].nome, sizeof(teatro.assentos[3][2].nome), nome2); 
    // teatro.assentos[4][3].reservado = 1;
    // snprintf(teatro.assentos[4][3].nome, sizeof(teatro.assentos[4][3].nome), nome2);
    // teatro.assentos[4][4].reservado = 1;
    // snprintf(teatro.assentos[4][4].nome, sizeof(teatro.assentos[4][4].nome), nome2);

    mostrarOperacoes();
    printf("O que voce gostaria de fazer? ");
    int operacao;
    scanf("%d", &operacao);
    while (operacao!=0){
        while (operacao < 0 || operacao > 13){
            printf("Por favor, digite uma operacao valida: ");
            scanf("%d", &operacao); 
        }
        printf("\n");
        // declaração de variaveis que serão utilizadas no switch
        char nome[100], nomeArquivo[100], linha;
        int linhaInteiro, coluna, n;
        switch (operacao){
            case 1:
                printf("SALVAR ESTADO DO TEATRO\n\n");
                printf("Qual o nome do arquivo que voce deseja salvar (termine com .txt)?\n");
                getchar();
                fgets(nomeArquivo, 100, stdin);
                if (strlen(nomeArquivo)>0 && nomeArquivo[strlen(nomeArquivo)-1]=='\n') {
                    nomeArquivo[strlen(nomeArquivo)-1] = '\0';
                }
                if (salvarEstado(teatro, nomeArquivo)) {
                    printf("Estado do programa salvo com sucesso.\n");
                } else {
                    printf("Erro ao salvar o estado do programa.\n");
                }
                break;
            case 2:
                printf("CARREGAR ESTADO DO TEATRO\n\n");
                printf("Qual o nome do arquivo que voce deseja carregar (termine com .txt)?\n");
                getchar();
                fgets(nomeArquivo, 100, stdin);
                if (strlen(nomeArquivo)>0 && nomeArquivo[strlen(nomeArquivo)-1]=='\n') {
                    nomeArquivo[strlen(nomeArquivo)-1] = '\0';
                }
                teatro = carregarEstado(nomeArquivo);
                break;
            case 3: // Visualizar
                printf("VISUALIZAR TEATRO\n\n");
                exibirTeatro(teatro);
                break;
            case 4: // Reservar assento especifico
                printf("RESERVAR ASSENTO ESPECIFICO\n\n");
                printf("Qual o nome para a reserva?\n");
                getchar();
                fgets(nome, 100, stdin);
                if (strlen(nome)>0 && nome[strlen(nome)-1]=='\n') {
                    nome[strlen(nome)-1] = '\0';
                }
                int aux = 1;
                while (aux!=0){
                    printf("Qual a linha (letra maiuscula) do assento que voce deseja reservar? ");
                    scanf(" %c", &linha);
                    // verifica se é um numero válido de linhas
                    if (verificarLinhaValida(teatro, linha)){
                        linhaInteiro = converterCharParaInt(linha);
                    } else {
                        break;
                    }
                    printf("Qual a coluna (numero) do assento que voce deseja reservar? ");
                    scanf("%d", &coluna);
                    // verifica se é um numero válido de colunas
                    if (verificarColunaValida(teatro, coluna)==0){
                        break;
                    }
                    if (reservarAssento(teatro, linhaInteiro-1, coluna-1, nome)){
                        aux=0;
                    }  
                }            
                break; 
            case 5: // Reservar assentos consecutivos especificos
                printf("RESERVAR ASSENTOS CONSECUTIVOS ESPECIFICOS\n\n");
                printf("Qual o nome para as reservas?\n");
                getchar();
                fgets(nome, 100, stdin);
                if (strlen(nome)>0 && nome[strlen(nome)-1]=='\n') {
                    nome[strlen(nome)-1] = '\0';
                }
                printf("Qual o numero de assentos consecutivos que voce deseja reservar? ");
                scanf("%d", &n);
                printf("Qual a linha (letra maiuscula) dos assentos que voce deseja reservar? ");
                scanf(" %c", &linha);
                // verifica se é um numero válido de linhas
                if (verificarLinhaValida(teatro, linha)){
                    linhaInteiro = converterCharParaInt(linha);
                } else {
                    break;
                }
                printf("Qual a coluna (numero) do assento que voce deseja iniciar suas reservas? ");
                scanf("%d", &coluna);
                // verifica se é um numero válido de colunas
                if (verificarColunaValida(teatro, coluna)==0){
                    break;
                }
                if (reservarAssentosConsecutivos(teatro, linhaInteiro-1, coluna-1, n, nome)){
                    printf("Seus assentos em %c%d ate %c%d foram reservados com sucesso!\n", linha, coluna, linha, coluna+(n-1));
                } else {
                    printf("Nao foi possivel efetuar a reserva dos %d assentos consecutivos iniciando em %c%d.\n", n, linha, coluna);
                }
                break;
            case 6: // Reservar assento pelo sistema
                printf("RESERVAR ASSENTO PELO SISTEMA\n\n");
                printf("Qual o nome para a reserva?\n");
                getchar();
                fgets(nome, 100, stdin);
                if (strlen(nome)>0 && nome[strlen(nome)-1]=='\n') {
                    nome[strlen(nome)-1] = '\0';
                }
                Assento *assento = reservarAssentoPeloSistema(teatro, nome);
                if (assento==NULL) {
                    printf("Todos os assentos estao ocupados.\n");
                } else {
                    printf("Seu assento em %c%d foi reservado pelo sistema com sucesso!\n", assento->fileira, assento->numero);
                }
                break;
            case 7: // Reservar assentos consecutivos pelo sistema
                printf("RESERVAR ASSENTOS CONSECUTIVOS PELO SISTEMA\n\n");
                printf("Qual o nome para a reserva?\n");
                getchar();
                fgets(nome, 100, stdin);
                if (strlen(nome)>0 && nome[strlen(nome)-1]=='\n') {
                    nome[strlen(nome)-1] = '\0';
                }
                printf("Qual o numero de assentos consecutivos que voce deseja reservar? ");
                scanf("%d", &n);
                Assento **assentosConsecutivos = reservarAssentosConsecutivosPeloSistema(teatro, n, nome);
                if (assentosConsecutivos==NULL) {
                    printf("Nao ha %d assentos consecutivos livres.\n", n);
                } else {
                    printf("Seus assentos em %c%d ate %c%d foram reservados com sucesso!\n", assentosConsecutivos[0]->fileira, assentosConsecutivos[0]->numero, assentosConsecutivos[n-1]->fileira, assentosConsecutivos[n-1]->numero);
                    free(assentosConsecutivos);
                }
                break;
            case 8: // Cancelar reserva de um assento
                printf("CANCELAR RESERVA DE UM ASSENTO\n\n");
                printf("Qual a linha (letra maiuscula) do assento que voce deseja cancelar a reserva? ");
                scanf(" %c", &linha);
                // verifica se é um numero válido de linhas
                if (verificarLinhaValida(teatro, linha)){
                    linhaInteiro = converterCharParaInt(linha);
                } else {
                    break;
                }
                printf("Qual a coluna (numero) do assento que voce deseja cancelar a reserva? ");
                scanf("%d", &coluna);
                // verifica se é um numero válido de colunas
                if (verificarColunaValida(teatro, coluna)==0){
                    break;
                }
                if (cancelarReserva(teatro, linhaInteiro-1, coluna-1)){
                    printf("A reserva no assento em %c%d foi cancelada.\n", linha, coluna);
                } else {
                    printf("Nenhuma reserva no assento em %c%d.\n", linha, coluna+1);
                }
                break;
            case 9: // Cancelar reservas de uma pessoa
                printf("CANCELAR RESERVAS DE UMA PESSOA\n\n");
                printf("Qual o nome da pessoa que voce quer cancelar as reservas?\n");
                getchar();
                fgets(nome, 100, stdin);
                if (strlen(nome)>0 && nome[strlen(nome)-1]=='\n') {
                    nome[strlen(nome)-1] = '\0';
                }
                int numCancelamentos = cancelarReservas(teatro, nome);
                if (numCancelamentos==0){
                        printf("Nenhuma reserva de %s foi cancelada.\n", nome);
                    } else if (numCancelamentos==1){
                        printf("1 reserva de %s foi cancelada.\n", nome);
                    } else{
                        printf("%d reservas de %s foram canceladas.\n",numCancelamentos, nome);
                    }
                break;
            case 10: // Cancelar todas as reservas
                printf("CANCELAR TODAS AS RESERVAS\n\n");
                printf("Tem certeza que voce deseja cancelar todas as reservas do teatro?\n1-SIM\t\t2-NAO\n");
                int resp;
                printf("Resposta: ");
                scanf("%d", &resp);
                if (resp==1){
                    int numCancelamentos = cancelarTodasAsReservas(teatro);
                    if (numCancelamentos==0){
                        printf("Nenhuma reserva foi cancelada.\n");
                    } else if (numCancelamentos==1){
                        printf("1 reserva foi cancelada.\n");
                    } else{
                        printf("%d reservas foram canceladas.\n",numCancelamentos);
                    }
                }
                break;
            case 11: // Verificar disponibilidade
                printf("VERIFICAR DISPONIBILIDADE DE UM ASSENTO\n\n");
                printf("Qual a linha (letra maiuscula) do assento que voce deseja verificar? ");
                scanf(" %c", &linha);
                // verifica se é um numero válido de linhas
                if (verificarLinhaValida(teatro, linha)){
                    linhaInteiro = converterCharParaInt(linha);
                } else {
                    break;
                }
                printf("Qual a coluna (numero) do assento que voce deseja verificar? ");
                scanf("%d", &coluna);
                // verifica se é um numero válido de colunas
                if (verificarColunaValida(teatro, coluna)==0){
                    break;
                }
                if (verificarDisponibilidade(teatro, linhaInteiro-1, coluna-1)){
                    printf("O assento em %c%d esta livre!\n", linha, coluna);
                } else{
                    printf("O assento %c%d esta ocupado.\n", linha, coluna);
                }
                break;
            case 12: // Consultar assentos consecutivos
                printf("CONSULTAR ASSENTOS CONSECUTIVOS\n\n");
                printf("Quantos assentos consecutivos voce gostaria de consultar? ");
                scanf("%d", &n);
                consultarAssentosConsecutivos(teatro, n);
                break;
            case 13: // Exibir informacoes
                printf("INFORMACOES DO TEATRO\n\n");
                exibirInformacoes(teatro);
                break;
        }
        printf("\n");
        mostrarOperacoes();
        printf("O que voce gostaria de fazer? ");
        scanf("%d", &operacao);
    }
    printf("\nObrigado por usar o nosso sistema! Volte sempre!\n\n");
    liberarMatriz(teatro.assentos, teatro.fileira);
    return 0;
}