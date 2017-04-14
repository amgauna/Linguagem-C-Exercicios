struct addr
{
  char nome[30];
  char rua[40];
  char cidade[18];
  char estado[02];
  char cep[09];
  char lixao[10];
  struct addr *next;    /* ponteiro da prox entrada */
  struct addr *prior;   /* ponteiro p/ entrada anterior */
} lista;

struct addr *start;   /* primeiro da lista */
struct addr *last;    /* ultimo da lista */
struct addr *null;    /* endereco inicial */
struct addr *entra;   /* entrada atual */
char mostra = 'S';

main()
{
  int choice;
  cls;
  printf("Exibir Situacao (Enter Exibe - n nao): "); scanf("%c",&mostra);
  null = (struct addr *)malloc(sizeof(lista));
  do {
       choice = menu_select();
       switch(choice)
         {
           case 1 : enter();
                    break;
           case 2 : delete();
                    break;
           case 3 : list();
                    break;
           case 4 : achar();
                    break;
           case 5 : save();
                    break;
           case 6 : load();
                    break;
           case 7 : exit(0);
         }
      } while(1);
} 

menu_select()
{
  char s[80];
  int c;
  printf("1. Inserir Nomes\n");     
  printf("2. Retirar Nomes\n");     
  printf("3. Listar  Nomes\n");     
  printf("4. Procurar Nome\n");     
  printf("5. Salvar Arquivo\n");     
  printf("6. Carregar Arquivo\n");     
  printf("7. Finalizar\n");     
  do {
     printf("\nSua opcao: ");
     gets(s);
     c = atoi(s);
     } while (c < 0 || c > 7);
  return c;
}

enter()
{ 
  struct addr *info;
  struct addr *old;
  int situacao;
  char lixo;
  char *malloc();
  do {
     last = (struct addr *)malloc(sizeof(lista));
     if(last == 0) {
       printf("\nMemoria Esgotada!");
       return;
     }
     inputs("Entre Nome: ",last->nome,30);
     if (!last->nome[0]) break;
     inputs("Entre Rua: ",last->rua,40);
     inputs("Entre Cidade: ",last->cidade,18);
     inputs("Entre Estado: ",last->estado,02);
     inputs("Entre CEP: ",last->cep,10);
     if(start!=0) {
       info = start;
       old = start;
       situacao = 0;
       while (situacao==0) {
         if(mostra!='n') {
           printf("Posicionado em %s  <Enter>\n",info->nome);
           scanf("%c",&lixo);
         }
         if(*info->nome>*last->nome) {
           if(mostra != 'n') {
             printf("Posicionou (1) %s\n",info->nome);
             situacao = 1;
             break;
           }
           if(info==null) {
             if (mostra != 'n')
                printf("Fim de Arquivo (2) %s\n",last->nome);
             situacao = 2;
             break;
           }
         }
         old = info;
         info = info->next;
         if(info->next==null) {
           if(mostra != 'n') {
             printf("Aponta para Fim de Arquivo (3) %s\n",info->nome);
             situacao = 3;
             break;
           }
         }
       }
       if(situacao==1) {
         if(info==start) {
           last->next = info;
           info->prior = last;
           last->prior = null;
           start = last;
           }
         else {
           last->next = info;
           info->prior = last;
           last->prior = old;
           old->next = last;
           }
       }
       if(situacao==2) {
         last->next = null;
         old->next = last;
         last->prior = old;
       }
       if(situacao==3) {
         last->next = null;
         info->next = last;
         last->prior = info;
       }
     }
     else {
       start = last;
       start->next = null;
       start->prior = null;
     }
  }while(1);
}

inputs(prompt,s,count)
char *prompt;
char *s;
int count;
{
  char p[255];
  do {
     printf(prompt);
     gets(p);
     if (strlen(p) > count)
        puts("Muito Grande!");
    } while (strlen(p) > count);
  strcpy(s,p);
}

delete()
{
  struct addr *info, *find();
         char s[255];
  int volta;
  inputs("Entre Nome: ",s,30);
  info = find(s);
  if (info) {
     if (start == info) {
        start = info->next;
        start->prior = null; 
        }
     else {
        info->prior->next = info->next;
        if (info != last) 
           info->next->prior = info->prior;
        else
           last = info->prior;
     }
     free(info);
  }
}

achar()
{
  struct addr *info, *find();
         char s[255];
  int volta;
  inputs("Entre Nome: ",s,30);
  info = find(s);
  if (info==null)
     printf("Nao Encontrado!\n");
  else
     display(info);
}

struct addr *find(nome)
char *nome;
{
  struct addr *info;
  info = start;
  while (info && info != null) {
    if (!strcmp(nome,info->nome)) return info;
    info = info->next;
    }
  return null;
}

list()
{
  int t;
  char lixo;
  struct addr *info;
  info = start;
  while (info && info != null) {
    display(info);
    scanf("%c",&lixo);
    info = info->next;
    if(info->next==null) break;
    }
  printf("\n\n");
}

display(info)
struct addr *info;
{
  printf("\n\n***  E X I B I N D O   R E G I S T R O  ***\n");
  printf("%s\n",info->nome);
  printf("%s\n",info->rua);
  printf("%s\n",info->cidade);
  printf("%s\n",info->estado);
  printf("%s\n",info->cep);
  printf("\n\n");
}

save()
{
  int t,size;
  struct addr *info;
         char *p;
         FILE *fp;
  if ((fp = fopen("ALFA.DAT","w")) == 0) {
     puts("Falhou a Abertura!");
     exit(0);
    }
  printf("Salvando Arquivo\n");
  size = sizeof(lista);
  info = start;
  while (info && info != null) {
    p = (char *)info; /* convesao p/ ponteiro de caracter */
    for (t=0;t<size-1;++t) putc(*p++,fp); /* salva um byte */
    if(info->next==null) break;
    info = info->next; /* proximo */
    }
  putc(EOF,fp); /* marcando fim de arquivo ... opcional */
  fclose(fp);
}

load()
{
  int t,size;
  struct addr *info,*temp;
         char *p,*malloc();
         FILE *fp;
  if ((fp = fopen("ALFA.DAT","r")) == 0) {
     puts("Falhou Abertura");
     exit(0);
    }
  printf("Carregando Arquivo\n");
  size = sizeof(lista);
  start = (struct addr *)malloc(size);
  if (!start) {
     puts("Acabou Memoria! ");
     return;
  }
  info = start;
  p = (char *)info; /* ponteiro para caracter */
  while ((*p++ = getc(fp)) != EOF) {
    for (t=0;t<size-2;++t)
      *p++ = getc(fp); /* carrega byte a byte */
      info->next = (struct addr *)malloc(size); /* aloca mais memoria */
      if (!info->next) {
         printf("Memoria Esgotada!\n");
         return;
      }
      info->prior = temp;
      temp = info;
      info = info->next;
      if(info->next==null) break;
      p = (char *)info;
      if (info == null) break;
    }
  free(temp->next);
  temp->next = null;
  last = temp;
  start->prior = null;
  fclose(fp);
}
   








    
    
     
