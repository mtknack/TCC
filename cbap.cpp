#include <iostream>
#include <string.h>
#include <time.h>

#include "cbap.hpp"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))

char INST[50] = "inst";
int PESO_NAV = 100;
int PESO_BER = 1000;


//------------------------------------------------------------------------------
int main()
{
   // clock_t h;
   // Solucao sol;
   // double tempo;
   // char arq[50];
   // sprintf(arq, ".\\instancias\\%s.txt", INST);
   // ler_instancia(arq);
   // ordenar_navios(vetIndNavOrd_, vetHorCheNav_);
   // atualizar_dimensoes_bercos();
   // criar_solucao(sol);
   // calc_fo(sol);
   // verificar_solucao(sol);
   // strcpy(arq, "./instancias/inst1.sol");
   // escrever_solucao(sol, arq);

   // for (int j = 0; j < numBer_; j++){
   //    for (int i = 0; i < sol.maiorQtdNavBer; i++){
   //       printf("%d - ", sol.matSol[j][i]);
   //    }
   //    printf("\n");
   // }

   
   // TESTE UNICO DE INSTANCIA
   srand(time(NULL));
   testaVNS(1, 6);


   //COLETAR DADOS PARA A TABELA DO TRABALHO 2
   // srand(time(NULL));

   // char arq1[50], arq2[50];
   // for (int i = 1; i < 6; i++){
   //    for(int j = 1; j < 4;j++){
   //       tempMelhorSol = 0;
   //       testaVNS(i, j);
   //       printf("inst%d-vez%d MelhorTempFO = %f\n",i,j,tempMelhorSol);
   //    }
   // }
   


   //-------------------------
   //--- Coletar tempos da construtiva e c�lculo de FO
   /*h = clock();
   criar_solucao(sol);
   tempo = (clock() - (double)h) / CLOCKS_PER_SEC;
   calc_fo(sol);
   printf("\nSI --> FO: %d\tTempo: %.5f\n", sol.funObj, tempo);
   h = clock();
   for (int i = 0; i < 100000; i++)
      criar_solucao(sol);
   tempo = (clock() - (double)h) / CLOCKS_PER_SEC;
   printf("Tempo SI: %.5f\n", tempo);
   h = clock();
   for (int i = 0; i < 100000; i++)
      calc_fo(sol);
   tempo = (clock() - (double)h) / CLOCKS_PER_SEC;
   printf("Tempo FO: %.5f\n", tempo);*/
   //-------------------------
   //--- Testar entrada
   //strcpy(arq, "teste.txt");
   //testar_instancia(arq);
   //-------------------------
   //--- Testar dimens�es dos ber�os
   //for (int i = 0; i < numBer_; i++)
   //   printf("%d  -  %d\n", vetPosIniBer_[i], vetPosFinBer_[i]);
   //-------------------------
   //--- Testar ordena��o
   //for (int i = 0; i < numNav_; i++)
   //   printf("%d %d\n", vetIndNavOrd_[i]+1, vetHorCheNav_[vetIndNavOrd_[i]]);
   //-------------------------
   //--- Testar leitura de solu��o
   //sprintf(arq, "%s.sol", INST);
   //ler_solucao(sol, arq);
   //strcpy(arq, "");
   //escrever_solucao(sol, arq);
   //-------------------------

   return 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ajustar_hor_pos_berco(Solucao &s, const int &ber)
{
   // Quando um ber o for alterado, esse metodo devera  ser chamado para ajustar horario e 
   // posicao de todos os navios desse berco
   // Usar o inicializar_hor_pos_navios() como base, porem, apenas para o berco ber
   // Se vetTamLEBer_ == 0 ==> posicao e o inicio do berco
   // Senao Se vetTamLDBer_ == 0 ==> posicao o final do berco - o tamanho do navio
   // Senao o verificar sobreposicoes a esquerda e depois a direita

   // ordenar_nav_hor_che(s,ber);
   bool sob_esq, sob_dir;
   int n1, n2, n3, hor_atual;
   if (s.vetQtdNavBer[ber] > 0)
   {
      hor_atual = vetHorAbeBer_[ber];
      for (int j = 0; j < s.vetQtdNavBer[ber]; j++)
      {
         n1 = s.matSol[ber][j];
         //--- Definir os hor�rios
         s.vetHorAtrNav[n1] = MAX(vetHorCheNav_[n1], hor_atual);
         s.vetHorSaiNav[n1] = s.vetHorAtrNav[n1] + matTemAte_[ber][n1];
         //--- Definir as posi��es
         s.vetPosAtrNav[n1] = vetPosIniBer_[ber];
         s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
         sob_dir = true;
         sob_esq = true;

         bool precisa_esq = true, precisa_dir = true;
         
         if(vetTamLDBer_[ber] == 0){ // lado direto do berco for 0
            s.vetPosAtrNav[n1] = vetPosFinBer_[ber] - vetTamNav_[n1];
            s.vetPosFinNav[n1] = vetPosFinBer_[ber];
            if(ber == (numBer_-1))
               precisa_dir = false;
         }
         else if (vetTamLEBer_[ber] == 0){
            if(ber == 0)
            precisa_esq = false;
         }
                     
         while (sob_esq || sob_dir)
         {
            sob_esq = false;
            sob_dir = false;
            if(precisa_esq){
               for (int k = 0; k < s.vetQtdNavBer[ber-1]; k++) //Checar sobreposicao com os navios do berco anterior
               {
                  n2 = s.matSol[ber-1][k];
                  if (navios_sobrepostos(s, n1, n2))
                  {
                     sob_esq = true;
                     break;
                  }
                  
               }
               if (sob_esq)
               {
                  if (s.vetPosFinNav[n2] + vetTamNav_[n1] <= vetPosFinBer_[ber])
                  {
                     s.vetPosAtrNav[n1] = s.vetPosFinNav[n2];
                     s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
                  }
                  else
                  {
                     s.vetHorAtrNav[n1] = s.vetHorSaiNav[n2];
                     s.vetHorSaiNav[n1] = s.vetHorAtrNav[n1] + matTemAte_[ber][n1];
                     s.vetPosAtrNav[n1] = vetPosFinBer_[ber] - vetTamNav_[n1];
                     s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
                  }
               }
            }
            if (precisa_dir){
               for (int k = 0; k < s.vetQtdNavBer[ber+1]; k++)
               {
                  n3 = s.matSol[ber+1][k];
                  if (navios_sobrepostos(s, n1, n3))
                  {
                     sob_dir = true;
                     break;
                  }
               } //Checar sobreposicao com os navios do berco posterior
               if(sob_dir){
                  s.vetHorAtrNav[n1] = s.vetHorSaiNav[n3];
                  s.vetHorSaiNav[n1] = s.vetHorAtrNav[n1] + matTemAte_[ber][n1];
                  s.vetPosAtrNav[n1] = vetPosIniBer_[ber];
                  s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
               }
            }
         }
         hor_atual = s.vetHorSaiNav[n1];
      }
   }
}

//------------------------------------------------------------------------------
void ordenar_nav_hor_che(Solucao &s,int ber){
   int aux;
   int vetNavOrdNoBer[MAX_NAV];
   int vetCheNavOrdNoBer[MAX_NAV];
   for (int i = 0; i < s.vetQtdNavBer[ber]; i++){//Copia
      vetNavOrdNoBer[i] = s.matSol[ber][i];
      vetCheNavOrdNoBer[i] = vetHorCheNav_[s.matSol[ber][i]];
   }
   for (int i = 1; i < s.vetQtdNavBer[ber]; i++){ //Ordena
      for (int j = 0; j < s.vetQtdNavBer[ber]-i; j++){
         if (vetCheNavOrdNoBer[j] > vetCheNavOrdNoBer[j+1]){
            aux = vetCheNavOrdNoBer[j];
            vetCheNavOrdNoBer[j] = vetCheNavOrdNoBer[j+1];
            vetCheNavOrdNoBer[j+1] = aux;
            
            aux = vetNavOrdNoBer[j];
            vetNavOrdNoBer[j] = vetNavOrdNoBer[j+1];
            vetNavOrdNoBer[j+1] = aux;
         }
      }
   }

   for (int i = 0; i < s.vetQtdNavBer[ber]; i++){ //Atribui
      s.matSol[ber][i] = vetNavOrdNoBer[i];
   }
}
//------------------------------------------------------------------------------
void calc_fo(Solucao &s)
{
   int ber;
   s.temSer = s.vioHorMaxNav = s.vioHorFecBer = 0;
   for (int i = 0; i < numNav_; i++)
   {
      ber = s.vetIdBerNav[i];
      s.temSer += (s.vetHorAtrNav[i] - vetHorCheNav_[i] + matTemAte_[ber][i]);
      s.vioHorMaxNav += MAX(0, s.vetHorSaiNav[i] - vetHorMaxNav_[i]);
      s.vioHorFecBer += MAX(0, s.vetHorSaiNav[i] - vetHorFecBer_[ber]);
   }
   s.funObj = s.temSer + PESO_NAV * s.vioHorMaxNav + PESO_BER * s.vioHorFecBer;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void criar_solucao(Solucao &s)
{
   int nav, ber, aux;
   memset(&s.vetQtdNavBer, 0, sizeof(s.vetQtdNavBer));
   memset(&s.matSol, -1, sizeof(s.matSol));
   ber = 0;
   s.maiorQtdNavBer = 0;
   for (int i = 0; i < numNav_; i++)
   {
      nav = vetIndNavOrd_[i];
      if (matTemAte_[ber][nav] == 0)
      {
         aux = 0;
         while (matTemAte_[aux][nav] == 0)
            aux++;
         s.matSol[aux][s.vetQtdNavBer[aux]] = nav;
         s.vetIdBerNav[nav] = aux;
         s.vetQtdNavBer[aux]++;
      }
      else
      {
         s.matSol[ber][s.vetQtdNavBer[ber]] = nav;
         s.vetIdBerNav[nav] = ber;
         s.vetQtdNavBer[ber]++;
         ber++;
         if (ber == numBer_)
            ber = 0;
      }
   }
   for (int i = 0; i < numBer_; i++)
      if(s.vetQtdNavBer[i] >= s.maiorQtdNavBer)
         s.maiorQtdNavBer = s.vetQtdNavBer[i];

   
   inicializar_hor_pos_navios(s);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void inicializar_hor_pos_navios(Solucao &s)
{  
   // 1 - cabe no berco esq e dir
   //    1.1 - aloca PosIni ber = final do ber-1 e PosFinal ber = final do ber-1 + tamNav
   //    1.2 - repete tudo
   // 2 - cabe no esq, mas sobrepoe o dir, entao sob_esq
   //    2.1 - Sobe tempo com inicio do ber-1, e posInicial inicial do ber-1
   //    2.2 - ver se pode colar na posFinal do ber-1, se sim cola e ver se tem sobreposicao
   //    2.3 - repete o tudo

   bool sob_esq;
   int n1, n2, hor_atual;
   for (int i = 0; i < numBer_; i++)
      if (s.vetQtdNavBer[i] > 0)
      {
         hor_atual = vetHorAbeBer_[i];
         for (int j = 0; j < s.vetQtdNavBer[i]; j++)
         {
            n1 = s.matSol[i][j];
            //--- Definir os hor�rios
            s.vetHorAtrNav[n1] = MAX(vetHorCheNav_[n1], hor_atual);
            s.vetHorSaiNav[n1] = s.vetHorAtrNav[n1] + matTemAte_[i][n1];
            //--- Definir as posi��es
            s.vetPosAtrNav[n1] = vetPosIniBer_[i];
            s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
            if (i != 0)
            {
               sob_esq = true;
               while (sob_esq)
               {
                  sob_esq = false;
                  for (int k = 0; k < s.vetQtdNavBer[i-1]; k++) //Checar sobreposicao com os navios do berco anterior
                  {
                     n2 = s.matSol[i-1][k];
                     if (navios_sobrepostos(s, n1, n2))
                     {
                        sob_esq = true;
                        break;
                     }
                  }
                  if (sob_esq)
                  {
                     if (s.vetPosFinNav[n2] + vetTamNav_[n1] <= vetPosFinBer_[i])
                     {
                        s.vetPosAtrNav[n1] = s.vetPosFinNav[n2];
                        s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
                     }
                     else
                     {
                        s.vetHorAtrNav[n1] = s.vetHorSaiNav[n2];
                        s.vetHorSaiNav[n1] = s.vetHorAtrNav[n1] + matTemAte_[i][n1];
                        s.vetPosAtrNav[n1] = vetPosFinBer_[i] - vetTamNav_[n1];
                        s.vetPosFinNav[n1] = s.vetPosAtrNav[n1] + vetTamNav_[n1];
                     }
                  }
               }
            }
            hor_atual = s.vetHorSaiNav[n1];
            s.vetHorAlocProxNav[i] = s.vetHorSaiNav[n1];
         }
      }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool  navios_sobrepostos(Solucao &s, const int &n1, const int &n2)
{
   if ((s.vetPosAtrNav[n1] < s.vetPosFinNav[n2]) && (s.vetPosFinNav[n1] > s.vetPosAtrNav[n2]) &&
      (s.vetHorAtrNav[n1] < s.vetHorSaiNav[n2]) && (s.vetHorSaiNav[n1] > s.vetHorAtrNav[n2]))
      return true;
   return false;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void verificar_solucao(Solucao &s)
{
   printf("\n\n>>> VERIFICACAO DA SOLUCAO <<<\n");
   bool viavel = true;
   //--- Verificar se todos os navios foram atendidos uma �nica vez
   int aux, vetAux[MAX_NAV];
   memset(&vetAux, 0, sizeof(vetAux));
   for(int i = 0; i < numBer_; i++)
      for (int j = 0; j < s.vetQtdNavBer[i]; j++)
      {
         aux = s.matSol[i][j];
         vetAux[aux]++;
      }
   aux = 0;
   for (int i = 0; i < numNav_; i++)
	   if (vetAux[i] != 1)
   	{
      printf(">>> Ha navios nao atendidos ou atendidos mais de uma vez!\n");
      viavel = false;
      break;
   }
   //-------------------------
   //--- Verificar se h� navios atracando em ber�os que n�o podem atend�-los
   for (int i = 0; i < numNav_; i++)
   {
      aux = s.vetIdBerNav[i];
      if (matTemAte_[aux][i] == 0)
      {
         printf("navio %d\n", i);
         printf(">>> Ha navios atracando em bercos que nao podem atende-los!\n");
         viavel = false;
         break;
      }
   }
   //-------------------------
   //--- Verificar se h� navios atracando antes de chegar ao porto
   for (int i = 0; i < numNav_; i++)
      if (s.vetHorAtrNav[i] < vetHorCheNav_[i])
      {
         printf(">>> Ha navios atracando antes de chegar ao porto!\n");
         viavel = false;
         break;
      }
   //-------------------------
   //--- Verificar se h� navios em atendimento ap�s o hor�rio limite
   for (int i = 0; i < numNav_; i++)
      if (s.vetHorSaiNav[i] > vetHorMaxNav_[i])
      {
         printf(">>> Ha navios em atendimento apos o seu horario limite!\n");
         viavel = false;
         break;
      }
   //-------------------------
   //--- Verificar se h� navios violando os hor�rios dos ber�os
   for (int i = 0; i < numNav_; i++)
   {
      aux = s.vetIdBerNav[i];
      if ((s.vetHorAtrNav[i] < vetHorAbeBer_[aux]) || (s.vetHorSaiNav[i] > vetHorFecBer_[aux]))
      {
         printf("%d\n", i);
         printf(">>> Ha navios violando os horarios dos bercos!\n");
         viavel = false;
         break;
      }
   }
   //-------------------------
   //--- Verificar se h� navios violando o tamanho dos ber�os
   for (int i = 0; i < numNav_; i++)
   {
      aux = s.vetIdBerNav[i];
      if ((s.vetPosAtrNav[i] < vetPosIniBer_[aux]) || (s.vetPosFinNav[i] > vetPosFinBer_[aux]))
      {
         printf("navio %d\n", i);
         printf(">>> Ha navios violando o tamanho dos bercos!\n");
         viavel = false;
         break;
      }
   }
   //-------------------------
   //--- Verificar se h� navios sobrepostos
   for (int i = 0; i < numNav_; i++)
      for (int j = 0; j < numNav_; j++)
         if (j != i)
            if(navios_sobrepostos(s, i, j))
            {
               printf(">>> Ha navios sobrepostos!\n");
               viavel = false;
               goto FIM;
            }
   //-------------------------
   FIM : ;
   if (viavel)
      printf("\n>>> SOLUCAO VIAVEL!\n");
   else{
      printf("\n>>> SOLUCAO INVIAVEL!\n");
      //s.funObj += 10000;
   }
      
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ler_solucao(Solucao &s, char *arq)
{
   char aux[30];
   int fo, temSer;
   FILE* f = fopen(arq, "r");
   fscanf(f, "%s %d\n", &aux, &fo);
   fscanf(f, "%s %s %s %d\n", &aux, &aux, &aux, &temSer);
   fscanf(f, "%s %s %s %s %s\n", &aux, &aux, &aux, &aux, &aux);
   for (int i = 0; i < numNav_; i++)
   {
      fscanf(f, "%s %d %d %d\n", &aux, &s.vetIdBerNav[i], &s.vetHorAtrNav[i], &s.vetPosAtrNav[i]);
      s.vetIdBerNav[i]--;
   }
   fclose(f);
   //--- Ordenar os navios pelo hor�rio de atraca��o
   int vetIndNavOrd[MAX_NAV];
   ordenar_navios(vetIndNavOrd, s.vetHorAtrNav);
   //--- Distribuir os navios nos ber�os (montar a matriz solu��o)
   memset(&s.vetQtdNavBer, 0, sizeof(s.vetQtdNavBer));
   int idNav, idBer;
   for (int i = 0; i < numNav_; i++)
   {
      idNav = vetIndNavOrd[i];
      idBer = s.vetIdBerNav[idNav];
      s.matSol[idBer][s.vetQtdNavBer[idBer]] = idNav;
      s.vetQtdNavBer[idBer]++;
   }
   //--- Calcular a FO e comparar os valores da solu��o lida
   // calc_fo(s);
   // if (s.funObj != fo)
   // {
   //    printf("\n>>> ERRO: valor da FO e diferente do calculado!\n");
   //    system("pause");
   // }
   // if (s.temSer != temSer)
   // {
   //    printf("\n>>> ERRO: valor do tempo de servico e diferente do calculado!\n");
   //    system("pause");
   // }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void escrever_solucao(Solucao &s, char *arq)
{
   FILE* f;
   if (strcmp(arq, "") == 0)
      f = stdout;
   else
      f = fopen(arq, "w");
   fprintf(f, "FO: %d\n", s.funObj);
   fprintf(f, "Tempo de servico: %d\n", s.temSer);
   fprintf(f, "--------------------------\n");
   fprintf(f, "Navio\tBerco\tTempo\tPosicao\n");
   for (int i = 0; i < numNav_; i++)
      fprintf(f, "%d\t%d\t%d\t%d\n", i+1, s.vetIdBerNav[i]+1, s.vetHorAtrNav[i], s.vetPosAtrNav[i]);
   fprintf(f, "\nMATRIZ DE SOLUCAO\n");
   for (int i = 0; i < numBer_; i++)
   {
      fprintf(f, "B%d: ", i+1);
      for (int j = 0; j < s.vetQtdNavBer[i]; j++)
         fprintf(f, "%d  ", s.matSol[i][j] + 1);
      fprintf(f, "\n");
   }
   if (strcmp(arq, "") != 0)
      fclose(f);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void clonar_solucao(Solucao &o, Solucao &c) 
{
   memcpy(&c, &o, sizeof(o));
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ordenar_navios(int *vetInd, int *vetVal)
{
   int flag, aux;
   for (int i = 0; i < numNav_; i++)
      vetInd[i] = i;
   flag = 1;
   while (flag)
   {
      flag = 0;
      for (int i = 0; i < numNav_ - 1; i++)
      {
         if (vetVal[vetInd[i]] > vetVal[vetInd[i + 1]])
         {
            flag = 1;
            aux = vetInd[i];
            vetInd[i] = vetInd[i + 1];
            vetInd[i + 1] = aux;
         }
      }
   }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void testar_instancia(char *arq)
{
   FILE* f = fopen(arq, "w");
   fprintf(f, "%d\t%d\n\n", numNav_, numBer_);
   for (int i = 0; i < numNav_; i++)
      fprintf(f, "%d\t", vetTamNav_[i]);
   fprintf(f, "\n\n");
   for (int i = 0; i < numNav_; i++)
      fprintf(f, "%d\t", vetHorCheNav_[i]);
   fprintf(f, "\n\n");
   for (int i = 0; i < numNav_; i++)
      fprintf(f, "%d\t", vetHorMaxNav_[i]);
   fprintf(f, "\n\n");
   for (int i = 0; i < numBer_; i++)
      fprintf(f, "%d\t%d\n", vetHorAbeBer_[i], vetHorFecBer_[i]);
   fprintf(f, "\n");
   for (int i = 0; i < numBer_; i++)
      fprintf(f, "%d\t%d\n", vetTamLEBer_[i], vetTamLDBer_[i]);
   fprintf(f, "\n");
   for (int i = 0; i < numBer_; i++)
   {
      for (int j = 0; j < numNav_; j++)
         fprintf(f, "%d\t", matTemAte_[i][j]);
      fprintf(f, "\n");
   }
   fclose(f);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void atualizar_dimensoes_bercos()
{
   vetPosIniBer_[0] = 0;
   vetPosFinBer_[0] = vetTamLEBer_[0] + vetTamLDBer_[0] + vetTamLEBer_[1];
   for (int i = 1; i < numBer_; i++)
   {
      vetPosIniBer_[i] = vetPosFinBer_[i-1] - vetTamLEBer_[i];
      vetPosFinBer_[i] = vetPosIniBer_[i] + vetTamLEBer_[i] + vetTamLDBer_[i];
      vetPosIniBer_[i] -= vetTamLDBer_[i-1];
      if (i != numBer_ -1)
         vetPosFinBer_[i] += vetTamLEBer_[i+1];
   }
   for (int i = 0; i < numBer_; i++)
      for (int j = 0; j < numNav_; j++)
         if (vetTamNav_[j] > (vetPosFinBer_[i] - vetPosIniBer_[i]))
            matTemAte_[i][j] = 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ler_instancia(char *arq)
{
   FILE *f = fopen(arq,"r");
   fscanf(f,"%d\t%d\n", &numNav_, &numBer_);
   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetTamNav_[i]);
   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetHorCheNav_[i]);
   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetHorMaxNav_[i]);
   for (int i = 0; i < numBer_; i++)
      fscanf(f, "%d %d", &vetHorAbeBer_[i], &vetHorFecBer_[i]);
   for (int i = 0; i < numBer_; i++)
      fscanf(f, "%d %d", &vetTamLEBer_[i], &vetTamLDBer_[i]);
   for (int i = 0; i < numBer_; i++)
      for (int j = 0; j < numNav_; j++)
         fscanf(f, "%d", &matTemAte_[i][j]);
   fclose(f);
}
//------------------------------------------------------------------------------

// novo ------------------------------------------------------------------------

// novo ------------------------------------------------------------------------

void testaVNS(int Inst, int Vez){
   // --------------------- Inicializando padrão ----------------------- //
   Solucao s;
   clock_t h;
   h = clock();

   double tempo;
   
   for (int j = 0; j < numBer_; j++){
      for (int i = 0; i < numNav_; i++){
         s.matSol[j][i] = -1;
      }
      printf("\n");
   }
   
   char arq[50];
   sprintf(arq, ".//instancias//%s%d.txt", INST, Inst);
   ler_instancia(arq);
   ordenar_navios(vetIndNavOrd_, vetHorCheNav_);
   atualizar_dimensoes_bercos();
   // criar_solucao(s);
   descobre_tam_total_dos_bercos();
   criar_solucao_por_tamanho(s);
   calc_fo(s);

   // printf("%d\n", s.maiorQtdNavBer);
   // for (int j = 0; j < numBer_; j++){
   //    for (int i = 0; i < s.vetQtdNavBer[j]; i++){
   //       printf("%d - ", s.matSol[j][i]);
   //    }
   //    printf("\n");
   //    printf("%d\n", s.vetQtdNavBer[j]);
   // }
   
   printf("Primeira FO = %d\n", s.funObj);
   printf("TempSer = %d\n", s.temSer);

   double ITmaxTempo = 240;//Segundos
   int qtd = -1;
   int qViz = 3;
   VNS(s, ITmaxTempo, qViz, qtd); 
   calc_fo(s);
   h = clock() - h; 
   tempo = (double)h/CLOCKS_PER_SEC;
   verificar_solucao(s);
   printf("MelFO = %d\n", s.funObj);
   printf("TempSer = %d\n", s.temSer);
   printf("Tempo = %lf\n", tempo);

   for (int j = 0; j < numBer_; j++){
      for (int i = 0; i < s.maiorQtdNavBer; i++){
         printf("%d - ", s.matSol[j][i]);
      }
      printf("\n");
   }
  
   sprintf(arq, ".//instancias//%s%d-%d-%d.sol", INST, Inst, Vez, s.funObj);
   escrever_solucao(s, arq);
}

void VNS(Solucao &S, double ITmaxTempo, int qViz, int qtd){
   Solucao S1;
   clock_t h;
   double tempExec = 0;
   int i=1,k;
   
   clonar_solucao(S,S1);
   
   while(tempExec <= ITmaxTempo || i <= qtd){
      h = clock();
      ++i;
      k = 1;
      while(k <= qViz){
         switch (k){
         case 1:
            gerarViz1(S1,true);
            break;
         case 2:
            gerarViz2(S1);
            break;
         case 3:
            gerarViz3(S1);
            break;
         case 4:
            gerarViz4(S1); // inconcluido, erro de travamento mas não deu tempo para achar
            break;
         }

         heuBLPM(S1); // Busca local primeira melhora
         if(S1.funObj < S.funObj){ // para maximizar use ">", para minimizar use "<"
            clonar_solucao(S1,S);
            k = 1;
            //printf("funObj - %d\n", S.funObj);
            tempMelhorSol = tempExec;
         }else{
            clonar_solucao(S,S1);
            // printf("N melhorou\n");
            k++;
         }
      }

      h = clock() - h;
      tempExec += (double)h/CLOCKS_PER_SEC;
      // printf("tempExec=%f / %f -  qtd=%d / %d\n",tempExec,ITmaxTempo, i,qtd);
   }
}


void gerarViz1(Solucao &S,bool att){
	int nav1, nav2, ber1, ber2, camada1, camada2, teste = true;

	while(teste){
      ber1 = rand()%(numBer_);
		ber2 = rand()%(numBer_);
		camada1 = (rand()%(S.vetQtdNavBer[ber1]));
		camada2 = (rand()%(S.vetQtdNavBer[ber2]));

      nav1 = S.matSol[ber1][camada1];
      nav2 = S.matSol[ber2][camada2];

      if(nav1 != nav2 && (nav1 != -1 || nav2 != -1) && (matTemAte_[ber1][nav2] != 0 && matTemAte_[ber2][nav1] != 0)){//(nav1 != -1 || nav2 != -1)
         teste = false;
      }
	}

	S.matSol[ber1][camada1] = nav2;
	S.matSol[ber2][camada2] = nav1;

   S.vetIdBerNav[nav2] = ber1;
   S.vetIdBerNav[nav1] = ber2;

   if(att){
      if (ber1 < ber2){
         ajustar_hor_pos_berco(S,ber1);
         ajustar_hor_pos_berco(S,ber2);
         
      }else{
         ajustar_hor_pos_berco(S,ber2);
         ajustar_hor_pos_berco(S,ber1);
      }
   }
   
   calc_fo(S);
}


void gerarViz2(Solucao &S){ //Troca somente um navio de berco aleatório
	int nav1, ber1, ber2, camada1, teste = true;

	while(teste){
      ber1 = rand()%(numBer_);
		camada1 = (rand()%(S.vetQtdNavBer[ber1]));
      nav1 = S.matSol[ber1][camada1];

		ber2 = rand()%(numBer_); //berco destino

      if(matTemAte_[ber2][nav1] != 0){
         teste = false;
      }
	}

   for (int z = camada1; z < S.vetQtdNavBer[ber1]; z++){
      S.matSol[ber1][z] = S.matSol[ber1][z+1];
   }
   S.vetQtdNavBer[ber1]--;

	S.matSol[ber2][S.vetQtdNavBer[ber2]] = nav1;
   S.vetQtdNavBer[ber2]++;

   S.vetIdBerNav[nav1] = ber2;
   if (ber1 < ber2){
      ajustar_hor_pos_berco(S,ber1);
      ajustar_hor_pos_berco(S,ber2);
   }else{
      ajustar_hor_pos_berco(S,ber2);
      ajustar_hor_pos_berco(S,ber1);
   }

   calc_fo(S);
}

void heuBLPM(Solucao &s) // Heuristica primeira melhora
{
   Solucao s1;
   clonar_solucao(s,s1);
	//int vetObjAux[MAX_NAV]; // usado para evitar determinismo na ordem de teste dos objetos
   // for(int ber1 = 0; j < numNav_; j++)
   //    vetObjAux[j] = j;

   INICIO: ;
   
   for(int j = 0; j < numBer_; j++){
      for (int k = 0; k < s1.vetQtdNavBer[j]; k++){
         for(int i = 0; i < numBer_; i++){
            int nav = s1.matSol[j][k];
            if ((i == j) || matTemAte_[i][nav]==0){
               continue;
            }

            for (int z = k; z < s1.vetQtdNavBer[j]; z++){
               s1.matSol[j][z] = s1.matSol[j][z+1];
            }
            s1.vetQtdNavBer[j]--;
            s1.matSol[i][s1.vetQtdNavBer[i]] = nav; 
            s1.vetQtdNavBer[i]++;
            s1.vetIdBerNav[nav] = i;
            
            //inicializar_hor_pos_navios(s1); 
            if (j < i){
               ajustar_hor_pos_berco(s1,j);
               ajustar_hor_pos_berco(s1,i);
            }else{
               ajustar_hor_pos_berco(s1,i);
               ajustar_hor_pos_berco(s1,j);   
            }
            
            calc_fo(s1);

            if(s1.funObj < s.funObj){
               clonar_solucao(s1,s);
               goto INICIO;
               
            }
            else{
               clonar_solucao(s,s1);
            }
         }
      }
   }
}


void gerarViz3(Solucao &S){ // troca dois navios numBer_ vezes sucessivamente

   int nav1, nav2, ber1, ber2, camada1, camada2, teste = true, roda = 0;

   while (numBer_ > roda){

      gerarViz1(S,false);
      roda++;
   }

   inicializar_hor_pos_navios(S);
   calc_fo(S);
}

void gerarViz4(Solucao &S){ //Troca somente um navio de berco aleatório varias vezes
	int nav1, ber1, ber2, camada1, roda = 0, teste = true;

   while (numBer_ >= roda){
      teste = true;
      while(teste){
         ber1 = rand()%(numBer_);
         camada1 = (rand()%(S.vetQtdNavBer[ber1]));
         nav1 = S.matSol[ber1][camada1];

         ber2 = rand()%(numBer_); // berco destino

         if(matTemAte_[ber2][nav1] != 0){
            teste = false;
         }
      }

      for (int z = camada1; z < S.vetQtdNavBer[ber1]; z++){
         S.matSol[ber1][z] = S.matSol[ber1][z+1];
      }
      S.vetQtdNavBer[ber1]--;
      

      S.matSol[ber2][S.vetQtdNavBer[ber2]] = nav1;
      S.vetQtdNavBer[ber2]++;

      S.vetIdBerNav[nav1] = ber2;
      roda++;
   }

   inicializar_hor_pos_navios(S);
   calc_fo(S);
}

void criar_solucao_por_tamanho(Solucao &s)
{
   int nav, ber, aux;
   memset(&s.vetQtdNavBer, 0, sizeof(s.vetQtdNavBer));
   memset(&s.matSol, -1, sizeof(s.matSol));

   s.maiorQtdNavBer = 0;
   for (int i = 0; i < numNav_; i++)
   {
      nav = vetIndNavOrd_[i];
      ber = verificaMenorTempoBer(s, nav);
      // printf("ber escolhido - %d   matTemAte_[ber][nav] %d\n", ber, matTemAte_[ber][nav]);

      // if(i == 3){
      //    break;
      // }
      if (matTemAte_[ber][nav] == 0)
      {
         aux = verificaMenorTempoBer(s, nav);
         // int temp = 0;
         while (matTemAte_[aux][nav] == 0){
            aux = verificaMenorTempoBer(s, nav);
            // temp++;
            // if(temp == 3){
            //    // ver bug em instancias grandes
            //    break;
            // }
         }
         
         // printf("ber escolhido - %d   matTemAte_[ber][nav] %d\n", ber, matTemAte_[ber][nav]);
            
         s.matSol[aux][s.vetQtdNavBer[aux]] = nav;
         s.vetIdBerNav[nav] = aux;
         s.vetQtdNavBer[aux]++;
      }
      else
      {
         s.matSol[ber][s.vetQtdNavBer[ber]] = nav;
         s.vetIdBerNav[nav] = ber;
         s.vetQtdNavBer[ber]++;
         // ber++;
         // if (ber == numBer_)
         //    ber = 0;
      }

      inicializar_hor_pos_navios(s);
   }
   for (int i = 0; i < numBer_; i++)
      if(s.vetQtdNavBer[i] >= s.maiorQtdNavBer)
         s.maiorQtdNavBer = s.vetQtdNavBer[i];

   
}

void descobre_tam_total_dos_bercos(){
   for (int i = 0; i < numBer_; i++){

      vetTamTotalBer_[i] = vetTamLEBer_[i] + vetTamLDBer_[i];
      if( i != 0 && vetTamLDBer_[i-1] != 0 ){
         vetTamTotalBer_[i] += vetTamLDBer_[i-1];
      } else if( i != numBer_ && vetTamLEBer_[i+1] != 0){
         vetTamTotalBer_[i] += vetTamLEBer_[i+1];
      }

      vetIdBerOrdTamTotal_[i] = i; // somente para preencher o vetor com os ids
      vetIdBercoMelhorTemp_[i] = i; // somente para preencher o vetor com os ids
   }

   // for (int i = 0; i < numBer_; i++)
   // {
   //    printf("%d - ", vetTamTotalBer_[i]);
   // }
   // printf("\n");
   // for (int i = 0; i < numBer_; i++)
   // {
   //    printf("%d - ", vetIdBerOrdTamTotal_[i]);
   // }

   // printf("\n");
   // ordernar_berco_asc();

   // for (int i = 0; i < numBer_; i++)
   // {
   //    printf("%d - ", vetTamTotalBer_[i]);
   // }
   // printf("\n");
   // for (int i = 0; i < numBer_; i++)
   // {
   //    printf("%d - ", vetIdBerOrdTamTotal_[i]);
   // }
   
}

void ordernar_berco_asc(){
   int auxTam, auxId;
   for (int i = 0; i < (numBer_ - 1); i++){
      for (int j = i; j < (numBer_); j++){
         if(vetTamTotalBer_[i] > vetTamTotalBer_[j]){
            auxTam = vetTamTotalBer_[i];
            vetTamTotalBer_[i] = vetTamTotalBer_[j];
            vetTamTotalBer_[j] = auxTam;

            auxId = vetIdBerOrdTamTotal_[i];
            vetIdBerOrdTamTotal_[i] = vetIdBerOrdTamTotal_[j];
            vetIdBerOrdTamTotal_[j] = auxId;
         }
      }
   }
}

int verificaMenorTempoBer(Solucao &s, int nav){
   
   int n = numBer_; // para não modificar o valor de numBer_
   int auxId, auxTam, vetAuxTemp[MAX_BER], vetAuxIdBerco[MAX_BER];
   memcpy(&vetAuxTemp, &s.vetHorAlocProxNav, sizeof(s.vetHorAlocProxNav));
   memcpy(&vetAuxIdBerco, &vetIdBercoMelhorTemp_, sizeof(vetIdBercoMelhorTemp_));

   // printf("nav %d\n", nav);

   for (int i = 0; i < (n - 1); i++){
      // printf("ber - %d   matTemAte_[ber][nav] %d\n i = %d j = %d vetAuxIdBerco[i] = %d\n", i, matTemAte_[vetAuxIdBerco[i]][nav]);
      for (int j = i; j < (n); j++){

         if(matTemAte_[vetAuxIdBerco[i]][nav] == 0){
            auxId = vetAuxIdBerco[n - 1];
            vetAuxIdBerco[n - 1] = vetAuxIdBerco[i];
            vetAuxIdBerco[i] = auxId;

            auxTam = vetAuxTemp[n - 1];
            vetAuxTemp[n - 1] = vetAuxTemp[i];
            vetAuxTemp[i] = auxTam;
            n--;
            i--;
            break;
         }
         else if(matTemAte_[vetAuxIdBerco[j]][nav] == 0){
            auxId = vetAuxIdBerco[n - 1];
            vetAuxIdBerco[n - 1] = vetAuxIdBerco[j];
            vetAuxIdBerco[j] = auxId;

            auxTam = vetAuxTemp[n - 1];
            vetAuxTemp[n - 1] = vetAuxTemp[j];
            vetAuxTemp[j] = auxTam;
            n--;
            j--;
         }
         else if(
            vetAuxTemp[i] + matTemAte_[vetAuxIdBerco[i]][nav] > vetAuxTemp[j] + matTemAte_[vetAuxIdBerco[j]][nav]
         ){
            // printf("%d + %d > %d + %d\n", vetAuxTemp[i], matTemAte_[i][nav], vetAuxTemp[j], matTemAte_[j][nav]);
            auxId = vetAuxIdBerco[i];
            vetAuxIdBerco[i] = vetAuxIdBerco[j];
            vetAuxIdBerco[j] = auxId;

            auxTam = vetAuxTemp[i];
            vetAuxTemp[i] = vetAuxTemp[j];
            vetAuxTemp[j] = auxTam;
         }

         // printf("%d %d %d\n", i, j, n);
      }
   }

   // for (int i = 0; i < numBer_; i++){
   //    printf("%d - ", vetAuxIdBerco[i]);
   // }
   // printf("\n");
   
   return vetAuxIdBerco[ rand() % (numBer_/4 + 1)];
   // return vetAuxIdBerco[0];
}

// Função para mesclar dois subvetores de vetAuxTemp e vetAuxIdBerco[]
void merge(int arr[], int aux[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Cria vetores temporários
    int L[n1], R[n2];
    int L_id[n1], R_id[n2];

    // Copia os dados para os vetores temporários L[] e R[]
    for (i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        L_id[i] = aux[left + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[middle + 1 + j];
        R_id[j] = aux[middle + 1 + j];
    }

    // Mescla os vetores temporários de volta para arr[]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            aux[k] = L_id[i];
            i++;
        } else {
            arr[k] = R[j];
            aux[k] = R_id[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de L[], se houver algum
    while (i < n1) {
        arr[k] = L[i];
        aux[k] = L_id[i];
        i++;
        k++;
    }

    // Copia os elementos restantes de R[], se houver algum
    while (j < n2) {
        arr[k] = R[j];
        aux[k] = R_id[j];
        j++;
        k++;
    }
}

// Função principal do Merge Sort
void mergeSort(int arr[], int aux[], int left, int right) {
    if (left < right) {
        // Encontra o ponto médio
        int middle = left + (right - left) / 2;

        // Ordena a primeira e a segunda metades
        mergeSort(arr, aux, left, middle);
        mergeSort(arr, aux, middle + 1, right);

        // Mescla as duas metades ordenadas
        merge(arr, aux, left, middle, right);
    }
}
