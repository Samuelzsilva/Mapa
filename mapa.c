#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX_LIVROS 100 // Define o tamanho máximo para o array de livros.
#define MAX_USERS 100  // Define o tamanho máximo para o array de usuários.
#define MAX_EMPRESTIMOS 100 // Define o tamanho máximo para o array de empréstimos.

// ==============================
// ESTRUTURAS
// ==============================
struct cad_liv
{/* Código de identificação do título do livro, nome do autor, nome da editora ano de publicação, número
de exemplares disponíveis para empréstimo e status geral dos livros.*/
    int cod_livro;
    char titulo[100];
    char autor[80];
    char editora[60];
    int ano_pub;
    int exempl_disp;
    bool status;
};
struct cad_Users /* Número de matrícula único do usuário/aluno, nome completo do usuário, curso do usuário e número de
telefone para contato*/
{
    int matricula;
    char nome_completo[100];
    char curso[50];
    char telefone[15];
};
struct sistemaEmpt
{/* Código de identificação do empréstimo: matrícula do usuário que fez o empréstimo, código do livro
emprestado, data em que o livro foi emprestado, data prevista para devolução, data real em que o livro foi devolvido e
status do empréstimo: true se ativo ou false se devolvido.*/
    int cod_emprestimo;
    int matricula_usuario;
    int cod_livro;
    char data_emprestimo[11];
    char data_devolucao_prev[11];
    char data_devolucao_real[11];
    bool status;
};

// ==============================
// FUNÇÕES PARA ARQUIVOS
// ==============================

// Salva todos os dados da lista de livros no arquivo "livros.txt".
void salvarLivros(struct cad_liv *livros, int qtd)
{
    FILE *fs = fopen("livros.txt", "w"); // Abre o arquivo para escrita.
    if (!fs) return; // Se a abertura falhar, sai da função.
    int i;
    for(i = 0; i < qtd; i++)
        // Grava cada registro de livro no arquivo.
        fprintf(fs, "%d|%s|%s|%s|%d|%d|%d\n",
                livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                livros[i].ano_pub, livros[i].exempl_disp, livros[i].status);
    fclose(fs); // Fecha o arquivo.
}
// Carrega os dados de livros do arquivo "livros.txt" para o array 'livros'.
// Retorna a quantidade de livros carregados.
int carregarLivros(struct cad_liv *livros)
{
    FILE *fs = fopen("livros.txt", "r"); // Abre o arquivo para leitura.
    if (!fs) return 0; // Se a abertura falhar, retorna 0 (nenhum livro carregado).
    int i = 0;
    while (fscanf(fs, "%d|%99[^|]|%79[^|]|%59[^|]|%d|%d|%d\n",
                  &livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                  &livros[i].ano_pub, &livros[i].exempl_disp, &livros[i].status) == 7)
        i++; // Incrementa o contador de livros carregados.
    fclose(fs);
    return i; // Retorna a quantidade total de livros carregados.
}
// Salva todos os dados da lista de usuários no arquivo "usuarios.txt".
void salvarUsuarios(struct cad_Users *usuarios, int qtd)
{
    FILE *fs = fopen("usuarios.txt", "w");
    if (!fs) return;
    int i;
    for(i = 0; i < qtd; i++)
        // Grava cada registro de usuário no arquivo.
        fprintf(fs, "%d|%s|%s|%s\n",
                usuarios[i].matricula, usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
    fclose(fs);
}
// Carrega os dados de usuários do arquivo "usuarios.txt" para o array 'usuarios'.
// Retorna a quantidade de usuários carregados.
int carregarUsuarios(struct cad_Users *usuarios)
{
    FILE *fs = fopen("usuarios.txt", "r");
    if (!fs) return 0;
    int i = 0;
    // Lê cada linha do arquivo.
    while (fscanf(fs, "%d|%99[^|]|%49[^|]|%14[^\n]\n",
                  &usuarios[i].matricula, usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone) == 4)
        i++;
    fclose(fs);
    return i;
}
// Salva todos os dados da lista de empréstimos no arquivo "emprestimos.txt".
void salvarEmprestimos(struct sistemaEmpt *emprestimos, int qtd)
{
    FILE *f = fopen("emprestimos.txt", "w");
    if (!f) return;
    int i;
    for(i = 0; i < qtd; i++)
        // Grava cada registro de empréstimo no arquivo.
        fprintf(f, "%d|%d|%d|%s|%s|%s|%d\n",
                emprestimos[i].cod_emprestimo, emprestimos[i].matricula_usuario, emprestimos[i].cod_livro,
                emprestimos[i].data_emprestimo, emprestimos[i].data_devolucao_prev,
                emprestimos[i].data_devolucao_real, emprestimos[i].status);
    fclose(f);
}
// Carrega os dados de empréstimos do arquivo "emprestimos.txt" para o array 'emprestimos'.
// Retorna a quantidade de empréstimos carregados.
int carregarEmprestimos(struct sistemaEmpt *emprestimos)
{
    FILE *f = fopen("emprestimos.txt", "r");
    if (!f) return 0;
    int i = 0;
    // Lê cada linha do arquivo.
    while (fscanf(f, "%d|%d|%d|%10[^|]|%10[^|]|%10[^|]|%d\n",
                  &emprestimos[i].cod_emprestimo, &emprestimos[i].matricula_usuario,
                  &emprestimos[i].cod_livro, emprestimos[i].data_emprestimo,
                  emprestimos[i].data_devolucao_prev, emprestimos[i].data_devolucao_real,
                  &emprestimos[i].status) == 7)
        i++;
    fclose(f);
    return i;
}

// ==============================
// FUNÇÕES DE CADASTRO / PESQUISA / EMPRÉSTIMO
// ==============================

// Solicita dados do novo livro ao usuário e adiciona-o ao array 'livros'.
void cadastrarLivro(struct cad_liv *livros, int *qtdLivros)
{
    struct cad_liv l;
    printf("Codigo do livro: "); scanf("%d", &l.cod_livro); getchar(); // Lê o código do livro.
    printf("Titulo: "); fgets(l.titulo, 100, stdin); l.titulo[strcspn(l.titulo,"\n")] = 0; /* Lê o
    título.*/
    printf("Autor: "); fgets(l.autor, 80, stdin); l.autor[strcspn(l.autor,"\n")] = 0; // Lê o autor.
    printf("Editora: "); fgets(l.editora, 60, stdin); l.editora[strcspn(l.editora,"\n")] = 0; /* Lê
    a editora*/
    printf("Ano de publicacao: "); scanf("%d", &l.ano_pub); // Lê o ano de publicação.
    printf("Exemplares disponiveis: "); scanf("%d", &l.exempl_disp); // Lê a quantidade de exemplares disponíveis
    l.status = true; // Define o status como ativo.

    livros[*qtdLivros] = l; // Adiciona o novo livro no final do array.
    (*qtdLivros)++; // Incrementa o contador de livros.
    printf("\nLivro cadastrado com sucesso!\n");
}
// Solicita dados do novo usuário ao sistema e adiciona-o ao array 'usuarios'.
void cadastrarUsuario(struct cad_Users *usuarios, int *qtdUsuarios)
{
    struct cad_Users u;
    printf("Matricula: "); scanf("%d", &u.matricula); getchar(); // Lê a matrícula.
    printf("Nome completo: "); fgets(u.nome_completo, 100, stdin);
    u.nome_completo[strcspn(u.nome_completo,"\n")] = 0; // Lê o nome.
    printf("Curso: "); fgets(u.curso, 50, stdin); u.curso[strcspn(u.curso,"\n")] = 0; // Lê o curso.
    printf("Telefone: "); fgets(u.telefone, 15, stdin);
    u.telefone[strcspn(u.telefone,"\n")] = 0; // Lê o telefone.

    usuarios[*qtdUsuarios] = u; // Adiciona o novo usuário no final do array.
    (*qtdUsuarios)++; // Incrementa o contador de usuários.
    printf("\nUsuario cadastrado com sucesso!\n");
}
// Permite pesquisar livros por Código, Título ou Autor.
void pesquisarLivros(struct cad_liv *livros, int qtdLivros)
{
    int op;
    printf("Pesquisar por: \n1-Codigo  2-Titulo  3-Autor: ");
    scanf("%d", &op); getchar();
    bool achou = false; // Flag para verificar se algum livro foi encontrado.

    if (op == 1) // Pesquisa por Código
    {
        int cod; printf("Codigo: "); scanf("%d", &cod);
        int i;
        for(i=0; i<qtdLivros; i++)
            if (livros[i].cod_livro == cod) // Compara o código exato.
            {
                // Imprime os detalhes do livro encontrado.
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    } else if (op == 2) // Pesquisa por Título
    {
        char t[100]; printf("Titulo: "); fgets(t,100,stdin); t[strcspn(t,"\n")] = 0;
        int i;
        for(i=0; i<qtdLivros; i++)
            if (strstr(livros[i].titulo,t)) // Usa strstr para buscar a substring do título (busca parcial).
            {
                // Imprime os detalhes do livro encontrado.
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    } else if (op == 3) // Pesquisa por Autor
    {
        char a[80]; printf("Autor: "); fgets(a,80,stdin); a[strcspn(a,"\n")] = 0;
        int i;
        for(i=0; i<qtdLivros; i++)
            if (strstr(livros[i].autor,a)) // Usa strstr para buscar a substring do autor (busca parcial).
            {
                // Imprime os detalhes do livro encontrado.
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    }
    if (!achou) printf("Nenhum livro encontrado.\n"); // Informa se a pesquisa não teve resultados.
}
// Permite pesquisar usuários por Matrícula ou Nome.
void pesquisarUsuarios(struct cad_Users *usuarios, int qtdUsuarios)
{
    int op;
    printf("Pesquisar por:\n1-Matricula  2-Nome: ");
    scanf("%d", &op); getchar();
    bool achou = false; // Flag para verificar se algum usuário foi encontrado.

    if (op == 1) // Pesquisa por Matrícula
    {
        int mat; printf("Matricula: "); scanf("%d", &mat);
        int i;
        for(i=0; i<qtdUsuarios; i++)
            if (usuarios[i].matricula == mat) // Compara a matrícula exata.
            {
                // Imprime os detalhes do usuário encontrado.
                printf("%d | %s | %s | %s\n", usuarios[i].matricula,
                       usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
                achou = true;
            }
    } else if (op == 2) // Pesquisa por Nome
    {
        char n[100]; printf("Nome: "); fgets(n,100,stdin); n[strcspn(n,"\n")] = 0;
        int i;
        for(i=0; i<qtdUsuarios; i++)
            if (strstr(usuarios[i].nome_completo,n))
            {
                // Imprime os detalhes do usuário encontrado.
                printf("%d | %s | %s | %s\n", usuarios[i].matricula,
                       usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
                achou = true;
            }
    }
    if (!achou) printf("Nenhum usuario encontrado.\n"); // Informa se a pesquisa não teve resultados.
}
// Registra um novo empréstimo no sistema.
void realizarEmprestimo(struct sistemaEmpt *emprestimos, int *qtdEmp,
    struct cad_Users *usuarios, int qtdUsuarios, struct cad_liv *livros, int qtdLivros)
{
    int mat, cod;
    printf("Matricula do usuario: "); scanf("%d", &mat);
    int idxU = -1, idxL = -1;
    int i;
    // Busca pelo usuário.
    for(i=0; i<qtdUsuarios; i++) if (usuarios[i].matricula == mat) idxU = i;
    if (idxU == -1) { printf("Usuario não encontrado.\n"); return; }

    printf("Codigo do livro: "); scanf("%d", &cod);
    // Busca pelo livro.
    for(i=0; i<qtdLivros; i++) if (livros[i].cod_livro == cod) idxL = i;
    if (idxL == -1) { printf("Livro nao encontrado.\n"); return; }
    // Verifica disponibilidade.
    if (livros[idxL].exempl_disp <= 0) { printf("Sem exemplares disponiveis.\n"); return; }

    struct sistemaEmpt e;
    e.cod_emprestimo = *qtdEmp + 1; // Atribui um novo código de empréstimo sequencial.
    e.matricula_usuario = mat;
    e.cod_livro = cod;

    // Calcula as datas.
    time_t t = time(NULL); // Obtém o tempo atual.
    struct tm tm = *localtime(&t); // Converte para a estrutura de tempo local.
    // Formata a data de empréstimo.
    sprintf(e.data_emprestimo,"%02d/%02d/%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    tm.tm_mday += 7; mktime(&tm); // Adiciona 7 dias e recalcula a estrutura de tempo.
    // Formata a data de devolução prevista.
    sprintf(e.data_devolucao_prev,"%02d/%02d/%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    strcpy(e.data_devolucao_real,"--/--/----"); // Define a data de devolução real como não preenchida.
    e.status = true; // Empréstimo ativo.

    emprestimos[*qtdEmp] = e; // Adiciona o novo empréstimo.
    (*qtdEmp)++;
    livros[idxL].exempl_disp--; // Diminui o número de exemplares disponíveis do livro.
    printf("Emprestimo realizado!\n");
}
// Registra a devolução de um livro.
void realizarDevolucao(struct sistemaEmpt *emprestimos, int qtdEmp, struct cad_liv *livros, int qtdLivros)
{
    int codEmp;
    printf("Codigo do emprestimo: "); scanf("%d", &codEmp);
    int idx = -1;
    int i;
    // Busca pelo empréstimo ativo pelo código.
    for(i=0; i<qtdEmp; i++)
        if (emprestimos[i].cod_emprestimo == codEmp && emprestimos[i].status) idx = i;
    if (idx == -1) { printf("Emprestimo nao encontrado ou ja devolvido.\n"); return; }

    // obtém a data atual do sistema para registrar devolução
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    // Registra a data de devolução real.
    sprintf(emprestimos[idx].data_devolucao_real,"%02d/%02d/%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);

    // Atualiza o status do empréstimo e a disponibilidade do livro.
    emprestimos[idx].status = false; // Empréstimo torna-se inativo (devolvido).
    for(i=0; i<qtdLivros; i++)
        if (livros[i].cod_livro == emprestimos[idx].cod_livro) livros[i].exempl_disp++;
    printf("Devolucao registrada com sucesso!\n");
}
// Lista empréstimos, podendo filtrar por ativos ou devolvidos.
void listarEmprestimos(struct sistemaEmpt *emprestimos, int qtdEmp,
    struct cad_Users *usuarios, int qtdUsuarios, struct cad_liv *livros, int qtdLivros, bool ativos)
{
    printf("\n=== Emprestimos %s ===\n", ativos ? "Ativos" : "Devolvidos");
    int i;
    for(i=0; i<qtdEmp; i++)
    {
        if (emprestimos[i].status == ativos) // Filtra pela flag 'ativos'.
        {
            int idxU = -1, idxL = -1;
            int j;
            // Busca os dados do usuário e do livro para exibir o nome/título.
            for(j=0; j<qtdUsuarios; j++) if (usuarios[j].matricula == emprestimos[i].matricula_usuario) idxU = j;
            for(j=0; j<qtdLivros; j++) if (livros[j].cod_livro == emprestimos[i].cod_livro) idxL = j;
            // Imprime os dados do empréstimo.
            printf("Emp %d | Usuario: %s | Livro: %s | Emp: %s | Prev: %s | Real: %s\n",
                   emprestimos[i].cod_emprestimo,
                   idxU>=0?usuarios[idxU].nome_completo:"Desconhecido",
                   idxL>=0?livros[idxL].titulo:"Desconhecido",
                   emprestimos[i].data_emprestimo,
                   emprestimos[i].data_devolucao_prev,
                   emprestimos[i].data_devolucao_real);
        }
    }
}

// ==============================
// FUNÇÃO DE BACKUP
// ==============================

// Cria uma cópia de segurança de um arquivo para outro.
void backupArquivo(const char *origem, const char *destino)
{
    FILE *orig = fopen(origem, "rb"); // Abre o arquivo de origem para leitura binária.
    if (!orig) return;
    FILE *dest = fopen(destino, "wb"); // Abre o arquivo de destino para escrita binária.
    if (!dest) { fclose(orig); return; }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), orig)) > 0)
        fwrite(buffer, 1, bytes, dest);

    fclose(orig); // Fecha ambos os arquivos.
    fclose(dest);
    printf("Backup criado: %s ---> %s\n", origem, destino);
}

// ==============================
// MENUS
// ==============================
// Exibe o submenu de Cadastros e retorna a opção escolhida.
int menuCadastros()
{
    int opc;
    printf("\n--- MENU DE CADASTROS ---\n");
    printf("1 - Cadastrar Livro\n");
    printf("2 - Cadastrar Usuario\n");
    printf("0 - Voltar\n> ");
    scanf("%d", &opc); getchar();
    return opc;
}
// Exibe o submenu de Pesquisas e retorna a opção escolhida.
int menuPesquisas()
{
    int opc;
    printf("\n--- MENU DE PESQUISAS ---\n");
    printf("1 - Pesquisar Livros\n");
    printf("2 - Pesquisar Usuarios\n");
    printf("0 - Voltar\n> ");
    scanf("%d", &opc); getchar();
    return opc;
}
// Exibe o submenu de Empréstimos e Devoluções e retorna a opção escolhida.
int menuEmprestimos()
{
    int opc;
    printf("\n--- MENU DE EMPRESTIMOS ---\n");
    printf("1 - Realizar Emprestimo\n");
    printf("2 - Registrar Devolucao\n");
    printf("0 - Voltar\n> ");
    scanf("%d", &opc); getchar();
    return opc;
}
// Exibe o submenu de Listagens e retorna a opção escolhida.
int menuListagens()
{
    int opc;
    printf("\n--- MENU DE LISTAGENS ---\n");
    printf("1 - Listar Emprestimos Ativos\n");
    printf("2 - Listar Emprestimos Devolvidos\n");
    printf("0 - Voltar\n> ");
    scanf("%d", &opc); getchar();
    return opc;
}

// ==============================
// MAIN
// ==============================
int main()
{
    // Declara os arrays de estruturas para armazenar os dados em memória.
    struct cad_liv livros[MAX_LIVROS];
    struct cad_Users usuarios[MAX_USERS];
    struct sistemaEmpt emprestimos[MAX_EMPRESTIMOS];

    // Carrega dados salvos dos arquivos
    int qtdLivros = carregarLivros(livros);       // Tenta carregar os livros e obtém a quantidade.
    int qtdUsuarios = carregarUsuarios(usuarios); // Tenta carregar os usuários e obtém a quantidade.
    int qtdEmp = carregarEmprestimos(emprestimos); // Tenta carregar os empréstimos e obtém a quantidade.

    int opcao;
    do
    {
        // Exibe o Menu Principal
        printf("\n===========================================\n\n");
        printf("        >>> SISTEMA DE BIBLIOTECA <<<      \n");
        printf("\n===========================================\n\n");
        printf("1 - Cadastros\n");
        printf("2 - Pesquisas\n");
        printf("3 - Emprestimos e Devolucoes\n");
        printf("4 - Listagens\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao)
        {
            case 1:
            { // SUBMENU DE CADASTROS (Livros e Usuários)
                int opCad;
                do
                {
                    opCad = menuCadastros(); // Exibe o submenu e obtém a opção.
                    switch(opCad)
                    {
                        case 1: cadastrarLivro(livros, &qtdLivros); break;
                        case 2: cadastrarUsuario(usuarios, &qtdUsuarios); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opCad != 0); // Repete até o usuário escolher "Voltar".
                break;
            }
            case 2:
            { // SUBMENU DE PESQUISAS (Livros e Usuários)
                int opPesq;
                do
                {
                    opPesq = menuPesquisas(); // Exibe o submenu e obtém a opção.
                    switch(opPesq)
                    {
                        case 1: pesquisarLivros(livros, qtdLivros); break;
                        case 2: pesquisarUsuarios(usuarios, qtdUsuarios); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opPesq != 0); // Repete até o usuário escolher "Voltar".
                break;
            }
            case 3:
            { // SUBMENU DE EMPRÉSTIMOS E DEVOLUÇÕES
                int opEmp;
                do
                {
                    opEmp = menuEmprestimos(); // Exibe o submenu e obtém a opção.
                    switch(opEmp)
                    {
                        case 1: realizarEmprestimo(emprestimos, &qtdEmp, usuarios,
                        qtdUsuarios, livros, qtdLivros); break;
                        case 2: realizarDevolucao(emprestimos, qtdEmp, livros, qtdLivros); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opEmp != 0); // Repete até o usuário escolher "Voltar".
                break;
            }
            case 4:
            { // SUBMENU DE LISTAGENS (Empréstimos Ativos/Devolvidos)
                int opList;
                do
                {
                    opList = menuListagens(); // Exibe o submenu e obtém a opção.
                    switch(opList)
                    {
                        case 1: listarEmprestimos(emprestimos, qtdEmp, usuarios, qtdUsuarios, livros,
                            qtdLivros, true); break; // true para ativos
                        case 2: listarEmprestimos(emprestimos, qtdEmp, usuarios, qtdUsuarios, livros,
                            qtdLivros, false); break; // false para devolvidos
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opList != 0); // Repete até o usuário escolher "Voltar".
                break;
            }
            case 0:
                // SALVA OS DADOS ATUAIS
                salvarLivros(livros, qtdLivros); // Persiste os dados de livros no arquivo.
                salvarUsuarios(usuarios, qtdUsuarios); // Persiste os dados de usuários no arquivo.
                salvarEmprestimos(emprestimos, qtdEmp); // Persiste os dados de empréstimos no arquivo.

                // FAZ BACKUP AUTOMÁTICO AO SAIR
                backupArquivo("livros.txt", "backup_livros.txt");
                backupArquivo("usuarios.txt", "backup_usuarios.txt");
                backupArquivo("emprestimos.txt", "backup_emprestimos.txt");

                printf("\nDados salvos e backup criado com sucesso! Encerrando o sistema...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0); // encerra o loop principal.

    return 0; // Indica que o programa terminou com sucesso.
}