#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_LIVROS 100
#define MAX_USERS 100
#define MAX_EMPRESTIMOS 100

// ==============================
// ESTRUTURAS
// ==============================
struct cad_liv
{
    int cod_livro;
    char titulo[100];
    char autor[80];
    char editora[60];
    int ano_pub;
    int exempl_disp;
    bool status;
};
struct cad_Users
{
    int matricula;
    char nome_completo[100];
    char curso[50];
    char telefone[15];
};
struct sistemaEmpt
{
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

void salvarLivros(struct cad_liv *livros, int qtd)
{
    FILE *fs = fopen("livros.txt", "w"); // grava toda a lista atual
    if (!fs) return;
    int i;
    for(i = 0; i < qtd; i++)
        fprintf(fs, "%d|%s|%s|%s|%d|%d|%d\n",
                livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                livros[i].ano_pub, livros[i].exempl_disp, livros[i].status);
    fclose(fs);
}
int carregarLivros(struct cad_liv *livros)
{
    FILE *fs = fopen("livros.txt", "r");
    if (!fs) return 0;
    int i = 0;
    while (fscanf(fs, "%d|%99[^|]|%79[^|]|%59[^|]|%d|%d|%d\n",
                  &livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                  &livros[i].ano_pub, &livros[i].exempl_disp, &livros[i].status) == 7)
        i++;
    fclose(fs);
    return i;
}
void salvarUsuarios(struct cad_Users *usuarios, int qtd)
{
    FILE *fs = fopen("usuarios.txt", "w");
    if (!fs) return;
    int i;
    for(i = 0; i < qtd; i++)
        fprintf(fs, "%d|%s|%s|%s\n",
                usuarios[i].matricula, usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
    fclose(fs);
}
int carregarUsuarios(struct cad_Users *usuarios)
{
    FILE *fs = fopen("usuarios.txt", "r");
    if (!fs) return 0;
    int i = 0;
    while (fscanf(fs, "%d|%99[^|]|%49[^|]|%14[^\n]\n",
                  &usuarios[i].matricula, usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone) == 4)
        i++;
    fclose(fs);
    return i;
}
void salvarEmprestimos(struct sistemaEmpt *emprestimos, int qtd)
{
    FILE *f = fopen("emprestimos.txt", "w");
    if (!f) return;
    int i;
    for(i = 0; i < qtd; i++)
        fprintf(f, "%d|%d|%d|%s|%s|%s|%d\n",
                emprestimos[i].cod_emprestimo, emprestimos[i].matricula_usuario, emprestimos[i].cod_livro,
                emprestimos[i].data_emprestimo, emprestimos[i].data_devolucao_prev,
                emprestimos[i].data_devolucao_real, emprestimos[i].status);
    fclose(f);
}
int carregarEmprestimos(struct sistemaEmpt *emprestimos)
{
    FILE *f = fopen("emprestimos.txt", "r");
    if (!f) return 0;
    int i = 0;
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

void cadastrarLivro(struct cad_liv *livros, int *qtdLivros)
{
    struct cad_liv l;
    printf("Codigo do livro: "); scanf("%d", &l.cod_livro); getchar();
    printf("Titulo: "); fgets(l.titulo, 100, stdin); l.titulo[strcspn(l.titulo,"\n")] = 0;
    printf("Autor: "); fgets(l.autor, 80, stdin); l.autor[strcspn(l.autor,"\n")] = 0;
    printf("Editora: "); fgets(l.editora, 60, stdin); l.editora[strcspn(l.editora,"\n")] = 0;
    printf("Ano de publicacao: "); scanf("%d", &l.ano_pub);
    printf("Exemplares disponiveis: "); scanf("%d", &l.exempl_disp);
    l.status = true;

    livros[*qtdLivros] = l;
    (*qtdLivros)++;
    printf("\nLivro cadastrado com sucesso!\n");
}
void cadastrarUsuario(struct cad_Users *usuarios, int *qtdUsuarios)
{
    struct cad_Users u;
    printf("Matricula: "); scanf("%d", &u.matricula); getchar();
    printf("Nome completo: "); fgets(u.nome_completo, 100, stdin); u.nome_completo[strcspn(u.nome_completo,"\n")] = 0;
    printf("Curso: "); fgets(u.curso, 50, stdin); u.curso[strcspn(u.curso,"\n")] = 0;
    printf("Telefone: "); fgets(u.telefone, 15, stdin); u.telefone[strcspn(u.telefone,"\n")] = 0;

    usuarios[*qtdUsuarios] = u;
    (*qtdUsuarios)++;
    printf("\nUsuario cadastrado com sucesso!\n");
}
void pesquisarLivros(struct cad_liv *livros, int qtdLivros)
{
    int op;
    printf("Pesquisar por: \n1-Codigo  2-Titulo  3-Autor: ");
    scanf("%d", &op); getchar();
    bool achou = false;

    if (op == 1)
    {
        int cod; printf("Codigo: "); scanf("%d", &cod);
        int i;
        for(i=0; i<qtdLivros; i++)
            if (livros[i].cod_livro == cod)
            {
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    } else if (op == 2)
    {
        char t[100]; printf("Titulo: "); fgets(t,100,stdin); t[strcspn(t,"\n")] = 0;
        int i;
        for(i=0; i<qtdLivros; i++)
            if (strstr(livros[i].titulo,t))
            {
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    } else if (op == 3)
    {
        char a[80]; printf("Autor: "); fgets(a,80,stdin); a[strcspn(a,"\n")] = 0;
        int i;
        for(i=0; i<qtdLivros; i++)
            if (strstr(livros[i].autor,a))
            {
                printf("%d | %s | %s | %s | %d | %d\n",
                       livros[i].cod_livro, livros[i].titulo, livros[i].autor, livros[i].editora,
                       livros[i].ano_pub, livros[i].exempl_disp);
                achou = true;
            }
    }
    if (!achou) printf("Nenhum livro encontrado.\n");
}
void pesquisarUsuarios(struct cad_Users *usuarios, int qtdUsuarios)
{
    int op;
    printf("Pesquisar por:\n1-Matricula  2-Nome: ");
    scanf("%d", &op); getchar();
    bool achou = false;

    if (op == 1)
    {
        int mat; printf("Matricula: "); scanf("%d", &mat);
        int i;
        for(i=0; i<qtdUsuarios; i++)
            if (usuarios[i].matricula == mat)
            {
                printf("%d | %s | %s | %s\n", usuarios[i].matricula,
                       usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
                achou = true;
            }
    } else if (op == 2)
    {
        char n[100]; printf("Nome: "); fgets(n,100,stdin); n[strcspn(n,"\n")] = 0;
        int i;
        for(i=0; i<qtdUsuarios; i++)
            if (strstr(usuarios[i].nome_completo,n))
            {
                printf("%d | %s | %s | %s\n", usuarios[i].matricula,
                       usuarios[i].nome_completo, usuarios[i].curso, usuarios[i].telefone);
                achou = true;
            }
    }
    if (!achou) printf("Nenhum usuario encontrado.\n");
}
void realizarEmprestimo(struct sistemaEmpt *emprestimos, int *qtdEmp, struct cad_Users *usuarios, int qtdUsuarios, struct cad_liv *livros, int qtdLivros)
{
    int mat, cod;
    printf("Matricula do usuario: "); scanf("%d", &mat);
    int idxU = -1, idxL = -1;
    int i;
    for(i=0; i<qtdUsuarios; i++) if (usuarios[i].matricula == mat) idxU = i;
    if (idxU == -1) { printf("Usuario não encontrado.\n"); return; }

    printf("Codigo do livro: "); scanf("%d", &cod);
    for(i=0; i<qtdLivros; i++) if (livros[i].cod_livro == cod) idxL = i;
    if (idxL == -1) { printf("Livro nao encontrado.\n"); return; }
    if (livros[idxL].exempl_disp <= 0) { printf("Sem exemplares disponiveis.\n"); return; }

    struct sistemaEmpt e;
    e.cod_emprestimo = *qtdEmp + 1;
    e.matricula_usuario = mat;
    e.cod_livro = cod;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(e.data_emprestimo,"%02d/%02d/%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    tm.tm_mday += 7; mktime(&tm);
    sprintf(e.data_devolucao_prev,"%02d/%02d/%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    strcpy(e.data_devolucao_real,"--/--/----");
    e.status = true;

    emprestimos[*qtdEmp] = e;
    (*qtdEmp)++;
    livros[idxL].exempl_disp--;
    printf("Emprestimo realizado!\n");
}
void realizarDevolucao(struct sistemaEmpt *emprestimos, int qtdEmp, struct cad_liv *livros, int qtdLivros)
{
    int codEmp;
    printf("Codigo do emprestimo: "); scanf("%d", &codEmp);
    int idx = -1;
    int i;
    for(i=0; i<qtdEmp; i++)
        if (emprestimos[i].cod_emprestimo == codEmp && emprestimos[i].status) idx = i;
    if (idx == -1) { printf("Emprestimo não encontrado.\n"); return; }

    strcpy(emprestimos[idx].data_devolucao_real, emprestimos[idx].data_devolucao_prev);
    emprestimos[idx].status = false;
    for(i=0; i<qtdLivros; i++)
        if (livros[i].cod_livro == emprestimos[idx].cod_livro) livros[i].exempl_disp++;
    printf("Devolucao registrada com sucesso!\n");
}
void listarEmprestimos(struct sistemaEmpt *emprestimos, int qtdEmp, struct cad_Users *usuarios, int qtdUsuarios, struct cad_liv *livros, int qtdLivros, bool ativos)
{
    printf("\n=== Emprestimos %s ===\n", ativos ? "Ativos" : "Devolvidos");
    int i;
    for(i=0; i<qtdEmp; i++)
    {
        if (emprestimos[i].status == ativos)
        {
            int idxU = -1, idxL = -1;
            int j;
            for(j=0; j<qtdUsuarios; j++) if (usuarios[j].matricula == emprestimos[i].matricula_usuario) idxU = j;
            for(j=0; j<qtdLivros; j++) if (livros[j].cod_livro == emprestimos[i].cod_livro) idxL = j;
            printf("Emp %d | Usuário: %s | Livro: %s | Emp: %s | Prev: %s | Real: %s\n",
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
void backupArquivo(const char *origem, const char *destino)
{
    FILE *orig = fopen(origem, "rb");
    if (!orig) return;
    FILE *dest = fopen(destino, "wb");
    if (!dest) { fclose(orig); return; }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), orig)) > 0)
        fwrite(buffer, 1, bytes, dest);

    fclose(orig);
    fclose(dest);
    printf("Backup criado: %s ---> %s\n", origem, destino);
}

// ==============================
// MENUS
// ==============================
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
    struct cad_liv livros[MAX_LIVROS];
    struct cad_Users usuarios[MAX_USERS];
    struct sistemaEmpt emprestimos[MAX_EMPRESTIMOS];

    // Carrega dados salvos dos arquivos
    int qtdLivros = carregarLivros(livros);
    int qtdUsuarios = carregarUsuarios(usuarios);
    int qtdEmp = carregarEmprestimos(emprestimos);

    int opcao;
    do
    {
        printf("\n===========================================\n");
        printf("        >>> SISTEMA DE BIBLIOTECA <<<      \n");
        printf("===========================================\n");
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
            { // SUBMENU DE CADASTROS
                int opCad;
                do
                {
                    opCad = menuCadastros();
                    switch(opCad)
                    {
                        case 1: cadastrarLivro(livros, &qtdLivros); break;
                        case 2: cadastrarUsuario(usuarios, &qtdUsuarios); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opCad != 0);
                break;
            }
            case 2:
            { // SUBMENU DE PESQUISAS
                int opPesq;
                do
                {
                    opPesq = menuPesquisas();
                    switch(opPesq)
                    {
                        case 1: pesquisarLivros(livros, qtdLivros); break;
                        case 2: pesquisarUsuarios(usuarios, qtdUsuarios); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opPesq != 0);
                break;
            }
            case 3:
            { // SUBMENU DE EMPRÉSTIMOS E DEVOLUÇÕES
                int opEmp;
                do
                {
                    opEmp = menuEmprestimos();
                    switch(opEmp)
                    {
                        case 1: realizarEmprestimo(emprestimos, &qtdEmp, usuarios, qtdUsuarios, livros, qtdLivros); break;
                        case 2: realizarDevolucao(emprestimos, qtdEmp, livros, qtdLivros); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opEmp != 0);
                break;
            }
            case 4:
            { // SUBMENU DE LISTAGENS
                int opList;
                do
                {
                    opList = menuListagens();
                    switch(opList)
                    {
                        case 1: listarEmprestimos(emprestimos, qtdEmp, usuarios, qtdUsuarios, livros, qtdLivros, true); break;
                        case 2: listarEmprestimos(emprestimos, qtdEmp, usuarios, qtdUsuarios, livros, qtdLivros, false); break;
                        case 0: break;
                        default: printf("Opcao invalida!\n");
                    }
                } while(opList != 0);
                break;
            }
            case 0:
                // FAZ BACKUP AUTOMÁTICO AO SAIR
                backupArquivo("livros.txt", "backup_livros.txt");
                backupArquivo("usuarios.txt", "backup_usuarios.txt");
                backupArquivo("emprestimos.txt", "backup_emprestimos.txt");

                // SALVA OS DADOS ATUAIS
                salvarLivros(livros, qtdLivros);
                salvarUsuarios(usuarios, qtdUsuarios);
                salvarEmprestimos(emprestimos, qtdEmp);

                printf("\nBackup e dados salvos com sucesso! Encerrando o sistema...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
