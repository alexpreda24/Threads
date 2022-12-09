//Preda Ioan Alexandru-313CB
#include "TCoada.h"
#include "TStiva.h"

#define MAX 10000
int returnare(int *v){
    int i;
    for( i = 1;i < 32368;i++)
    if(v[i] == 0)return i;
    return 0;
}
void ordonare(void *coada, void *y)
{
    Task *x1 = (Task *)y;
    void *aux = InitQ(sizeof(Task));
    if (!aux)
        return;
    void *elem = malloc(sizeof(Task));
    if (!elem)
        return;
    if (VidaQ(coada))
    {
        IntrQ(coada, y);
    }
    else
    {
        int k = 0;
        while (((TCoada *)coada)->ic != NULL && ((TCoada *)coada)->sc != NULL)
        {
            ExtrQ(coada, &elem);
            Task *x = (Task *)elem;
            if (k == 0)
            {
                if (x1->prior > x->prior)
                {
                    IntrQ(aux, y);
                    k = 1;
                }
                else if (x1->prior == x->prior)
                {
                    if (x->timp > x1->timp)
                    {
                        IntrQ(aux, y);
                        k = 1;
                    }
                    else if (x->timp == x1->timp)
                    {
                        if (x1->ID < x->ID)
                        {
                            k = 1;
                            IntrQ(aux, y);
                        }
                    }
                }
            }
            IntrQ(aux, (void *)x);
        }
        if (k == 0)
            IntrQ(aux, y);
        ConcatQ(coada, aux);
    }
}
void afisare(void *coada, FILE *out)
{
    void *aux = InitQ(sizeof(Task));
    if (!aux)
        return;
    void *elem = malloc(sizeof(Task));
    if (!elem)
        return;
    ExtrQ(coada, &elem);
    Task *x = (Task *)elem;
    if (!VidaQ(coada))
        fprintf(out, "(%u: priority = %d, remaining_time = %d)", x->ID, x->prior, x->timp);
    IntrQ(aux, elem);
    while (((TCoada *)coada)->ic != NULL && ((TCoada *)coada)->sc != NULL)
    {
        ExtrQ(coada, &elem);
        Task *x = (Task *)elem;
        if(x->ID != 0)fprintf(out, ",\n(%u: priority = %d, remaining_time = %d)", x->ID, x->prior, x->timp);
        IntrQ(aux, elem);
    }
    ConcatQ(coada, aux);
}
void printtask(int k, int *v, void *s, void *coada, int prioritate, int timpul, FILE *out)
{
    if (Vida(s))
        return;
    
    Task *y = malloc(sizeof(Task));
    if (!y)
        return;
    int i;
    for (i = 0; i < k; i++)
    {
        int p = returnare(v);
        y->ID = p;
        v[p] = 1;
        y->prior = prioritate;
        y->timp = timpul;
        y->crttimp = timpul;
        ordonare((void *)coada, (void *)y);
        fprintf(out, "Task created successfully : ID %d.\n", y->ID);
    }
}
void run(int timpul,int *v,int q,void *coada,void *coadarun,void *coadafini){
                int k = 0;
               
                void *aux =InitQ(sizeof(Task));
                if(!aux)return;
                void *elem=malloc(sizeof(Task));
                if(!elem)return;
                while (((TCoada *)coada)->ic != NULL && ((TCoada *)coada)->sc != NULL && k < (2 * q - 1)){
                       ExtrQ(coada,&elem);
                       k++;
                     
                       IntrQ(coadarun,&elem);
                }
                 while (((TCoada *)coadarun)->ic != NULL && ((TCoada *)coadarun)->sc != NULL){
                       ExtrQ(coadarun,&elem);
                       Task *x = (Task*)elem;
                       x->crttimp = x->crttimp - timpul;
                       if(x->crttimp > 0)IntrQ(aux,(void *)elem);
                       else {IntrQ(coadafini,(void *)x);
                       v[x->ID] = 0;
                       }
                }
                if(!VidaQ(aux))ConcatQ(coadarun,aux);

}
void get_task(short k, void *coada, void *coadarun, void *coadafinish, FILE *out)
{

    void *aux = InitQ(sizeof(Task));
    void *elem = malloc(sizeof(Task));
    if (!elem)
        return;
    while (((TCoada *)coada)->ic != NULL && ((TCoada *)coada)->sc != NULL)
    {
        ExtrQ(coada, &elem);
        Task *x = (Task *)elem;
        if (x->ID == k)
            fprintf(out, "Task %u is waiting (remaining_time = %d).\n", x->ID, x->timp);
        IntrQ(aux, (void *)x);
    }
    ConcatQ(coada, aux);
    while (((TCoada *)coadarun)->ic != NULL && ((TCoada *)coadarun)->sc != NULL)
    {
        ExtrQ(coadarun, &elem);
        Task *x = (Task *)elem;
        if (x->ID == k)
            fprintf(out, "Task %u is waiting (remaining_time = %d).\n", x->ID, x->timp);
        IntrQ(aux, (void *)x);
    }
    ConcatQ(coada, aux);
    while (((TCoada *)coadafinish)->ic != NULL && ((TCoada *)coadafinish)->sc != NULL)
    {
        ExtrQ(coadafinish, &elem);
        Task *x = (Task *)elem;
        if (x->ID == k)
            fprintf(out, "Task %u is waiting (remaining_time = %d).\n", x->ID, x->timp);
        IntrQ(aux, (void *)x);
    }

    ConcatQ(coadafinish, aux);
    free(elem);
}
void get_thread(int k, void *s, FILE *out)
{
    void *aux = InitS(sizeof(Thread));
    if (!aux)
        return;
    void *elem = malloc(sizeof(Thread));
    if (!elem)
        return;

    while (((TStiva *)s)->vf != NULL)
    {
        Pop(s, elem);
        Thread *x = (Thread *)elem;
        if (x->ID == k)
            fprintf(out, "Thread %d is idle.\n", k);
        Push(aux, elem);
    }
    Rastoarna(aux, s);
}

int main(int argc, char **input)
{
    size_t len = 0;
    int n, q, numara = 0,v[32368];
    int i;
    for(i = 0;i < 32368;i++)
    v[i] = 0;
    void *s;
    void *coada, *coadafini, *coadarun;
    FILE *in = fopen(input[1], "rt");
    FILE *out = fopen(input[2], "wt");
    char *line = NULL, *p = NULL;
    while (getline(&line, &len, in) != -1)
    {
        p = strtok(line, " ");
        if (p[strlen(p) - 1] == '\n')
            p[strlen(p) - 1] = '\0';
        if (numara == 0)
        {
            n = atoi(p);
        }
        if (numara == 1)
        {
            q = atoi(p);
            s = InitS(sizeof(Thread), 2 * q);
            Thread *x = malloc(n * sizeof(Thread));
            if (!x)
                return 0;
            int i;
            for (i = (2 * q) - 1; i >= 0; i--)
            {
                int x = i + 1;
                Push(s, &x);
            }
            coada = InitQ(sizeof(Task), MAX);
            coadarun = InitQ(sizeof(Task), MAX);
            coadafini = InitQ(sizeof(Task), MAX);
        }
        if (strcmp(p, "add_tasks") == 0)
        {
            p = strtok(NULL, " ");

            short k = atoi(p);
            //numarul de taskuri
            p = strtok(NULL, " ");
            int timpul = atoi(p); //timpul de executie
            p = strtok(NULL, " ");

            int prioritate = atoi(p);
            Task *y = malloc(sizeof(Task) * n);
            if (!y)
                return 0;

            printtask(k, v, (void *)s, (void *)coada, prioritate, timpul, out);
        }
        if (strcmp(p, "get_task") == 0)
        {
            p = strtok(NULL, " ");
            short IDtask = atoi(p);
            get_task(IDtask, (void *)coada, (void *)coadarun, (void *)coadafini, out);
        }
        if (strcmp(p, "get_thread") == 0)
        {
            p = strtok(NULL, " ");
            int ID = atoi(p);
            if (ID < 2 * q)
            {
                get_thread(ID, (void *)s, out);
            }
        }
        if (strcmp(p, "print") == 0)
        {
            p = strtok(NULL, " ");
            if (p[strlen(p) - 1] == '\n')
                p[strlen(p) - 1] = '\0';
            if (strcmp(p, "waiting") == 0)
            {
                fprintf(out, "====== Waiting queue =======\n[");
                afisare((void *)coada, out);
                fprintf(out, "]\n");
            }
            else if (strcmp(p, "running") == 0)
            {
                fprintf(out, "====== Running in parallel =======\n[");
                afisare((void *)coadarun, out);
                fprintf(out, "]\n");
            }
            else if (strcmp(p, "finished") == 0){
                 fprintf(out, "====== Finished queue =======\n[");
                afisare((void *)coadafini, out);
                fprintf(out, "]\n");
            }
        }
        if(strcmp(p, "run") == 0){
            p = strtok(NULL," ");
            
            if(p[strlen(p) - 1] == '\n')p[strlen(p) - 1]= '\0';
            int timpul = atoi(p);
            fprintf(out,"Running tasks for %d ms...\n",timpul);
            run(timpul,v,q,(void *)coada,(void *)coadarun,(void *)coadafini);
        }
        numara++;
    }
    free(line);
    fclose(in);
    fclose(out);
    return 0;
}