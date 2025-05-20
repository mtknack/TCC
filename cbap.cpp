#include <iostream>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "cbap.hpp"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

char INST[50] = "inst";
char INST_MOCCIA[50] = "i";
char INST_CONTINUO[50] = "f";
int PESO_NAV = 100;
int PESO_BER = 1000;

//------------------------------------------------------------------------------
int main()
{
   float alpha = 0.2;       // Porcentagem de escolha de soluções
   int qtd = 50;            // interações do VNS
   double ITmaxTempo = 120; // segundos

   for (int i = 1; i <= 30; i++)
   {
      for (int j = 1; j <= 10; j++)
      {
         iniciaGrasp(i, j, ITmaxTempo, alpha, qtd, 2);
      }
   }

   return 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void iniciaGrasp(int Inst, int Vez, double ITmaxTempo, float alpha, int qtd, int instGroup)
{

   Solucao s;
   char arq[50];
   double bestFOtime = 0;

   ler_instancias_TCC(arq, Inst, Vez, instGroup);
   ordenar_navios(vetIndNavOrd_, vetHorCheNav_);
   atualizar_dimensoes_bercos();
   descobre_tam_total_dos_bercos();

   int qtdVns = (numBer_ * numNav_) * qtd;

   grasp(s, ITmaxTempo, alpha, qtdVns, bestFOtime);

   escrever_instancias_TCC(arq, Inst, Vez, s.funObj, instGroup, alpha, qtd);
   escrever_instancias_arquivo_TCC(Inst, Vez, s.funObj, bestFOtime);
   escrever_solucao(s, arq);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void grasp(Solucao &s, double ITmaxTempo, float alpha, int qtd, double &bestFOtime)
{

   Solucao Sa, Si;
   srand(time(NULL));
   int melhor = 1000000;
   int qViz = 3;
   int temp = 0;

   clock_t h;
   double tempExec = 0;

   for (int i = 0; tempExec <= ITmaxTempo; i++)
   {
      h = clock();

      criar_solucao_por_tamanho(Si, alpha);
      calc_fo(Si);
      VNS(Si, qViz, qtd);
      verificar_solucao(Si, false);

      if (Si.funObj < melhor)
      {
         clonar_solucao(Si, Sa);
         melhor = Sa.funObj;
         bestFOtime = tempExec;
      }
      else
      {
         memset(&Si, 0, sizeof(Solucao));
      }

      h = clock() - h;
      tempExec += (double)h / CLOCKS_PER_SEC;
      temp++;
   }

   clonar_solucao(Sa, s);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void VNS(Solucao &S, int qViz, int qtd)
{
   Solucao S1;
   int i = 0, k = 1;

   clonar_solucao(S, S1);
   while (i <= qtd)
   {
      ++i;

      switch (k)
      {
      case 1:
         gerarViz1(S1, true);
         break;
      case 2:
         gerarViz2(S1);
         break;
      case 3:
         gerarViz3(S1);
         break;
      }

      if (S1.funObj < S.funObj)
      { // para maximizar use ">", para minimizar use "<"
         clonar_solucao(S1, S);
         k = 1;
      }
      else
      {
         clonar_solucao(S, S1);

         if (k < qViz)
         {
            k++;
         }
         else
         {
            k = 1;
         }
      }
   }
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

         if (vetTamLDBer_[ber] == 0)
         { // lado direto do berco for 0
            s.vetPosAtrNav[n1] = vetPosFinBer_[ber] - vetTamNav_[n1];
            s.vetPosFinNav[n1] = vetPosFinBer_[ber];
            if (ber == (numBer_ - 1))
               precisa_dir = false;
         }
         else if (vetTamLEBer_[ber] == 0)
         {
            if (ber == 0)
               precisa_esq = false;
         }

         while (sob_esq || sob_dir)
         {
            sob_esq = false;
            sob_dir = false;
            if (precisa_esq)
            {
               for (int k = 0; k < s.vetQtdNavBer[ber - 1]; k++) // Checar sobreposicao com os navios do berco anterior
               {
                  n2 = s.matSol[ber - 1][k];
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
            if (precisa_dir)
            {
               for (int k = 0; k < s.vetQtdNavBer[ber + 1]; k++)
               {
                  n3 = s.matSol[ber + 1][k];
                  if (navios_sobrepostos(s, n1, n3))
                  {
                     sob_dir = true;
                     break;
                  }
               } // Checar sobreposicao com os navios do berco posterior
               if (sob_dir)
               {
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
                  for (int k = 0; k < s.vetQtdNavBer[i - 1]; k++) // Checar sobreposicao com os navios do berco anterior
                  {
                     n2 = s.matSol[i - 1][k];
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
bool navios_sobrepostos(Solucao &s, const int &n1, const int &n2)
{
   if ((s.vetPosAtrNav[n1] < s.vetPosFinNav[n2]) && (s.vetPosFinNav[n1] > s.vetPosAtrNav[n2]) &&
       (s.vetHorAtrNav[n1] < s.vetHorSaiNav[n2]) && (s.vetHorSaiNav[n1] > s.vetHorAtrNav[n2]))
      return true;
   return false;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void verificar_solucao(Solucao &s, bool indicador)
{
   if (indicador)
   {
      printf("\n\n>>> VERIFICACAO DA SOLUCAO <<<\n");
   }
   bool viavel = true;
   //--- Verificar se todos os navios foram atendidos uma �nica vez
   int aux, vetAux[MAX_NAV];
   memset(&vetAux, 0, sizeof(vetAux));
   for (int i = 0; i < numBer_; i++)
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
            if (navios_sobrepostos(s, i, j))
            {
               printf(">>> Ha navios sobrepostos!\n");
               viavel = false;
               goto FIM;
            }
//-------------------------
FIM:;
   if (indicador)
   {
      if (viavel)
         printf("\n>>> SOLUCAO VIAVEL!\n");
      else
      {
         printf("\n>>> SOLUCAO INVIAVEL!\n");
         // s.funObj += 10000;
      }
   }
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
void atualizar_dimensoes_bercos()
{
   vetPosIniBer_[0] = 0;
   vetPosFinBer_[0] = vetTamLEBer_[0] + vetTamLDBer_[0] + vetTamLEBer_[1];
   for (int i = 1; i < numBer_; i++)
   {
      vetPosIniBer_[i] = vetPosFinBer_[i - 1] - vetTamLEBer_[i];
      vetPosFinBer_[i] = vetPosIniBer_[i] + vetTamLEBer_[i] + vetTamLDBer_[i];
      vetPosIniBer_[i] -= vetTamLDBer_[i - 1];
      if (i != numBer_ - 1)
         vetPosFinBer_[i] += vetTamLEBer_[i + 1];
   }
   for (int i = 0; i < numBer_; i++)
      for (int j = 0; j < numNav_; j++)
         if (vetTamNav_[j] > (vetPosFinBer_[i] - vetPosIniBer_[i]))
            matTemAte_[i][j] = 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void gerarViz1(Solucao &S, bool att)
{
   int nav1, nav2, ber1, ber2, camada1, camada2, teste = true;

   while (teste)
   {
      ber1 = rand() % (numBer_);
      ber2 = rand() % (numBer_);

      if (S.vetQtdNavBer[ber1] > 0)
      {
         camada1 = (rand() % (S.vetQtdNavBer[ber1]));
      }
      else
      {
         while (S.vetQtdNavBer[ber1] == 0)
         {
            ber1 = rand() % (numBer_);
         }
         camada1 = (rand() % (S.vetQtdNavBer[ber1]));
      }

      if (S.vetQtdNavBer[ber2] > 0)
      {
         camada2 = (rand() % (S.vetQtdNavBer[ber2]));
      }
      else
      {
         while (S.vetQtdNavBer[ber2] == 0)
         {
            ber2 = rand() % (numBer_);
         }
         camada2 = (rand() % (S.vetQtdNavBer[ber2]));
      }

      nav1 = S.matSol[ber1][camada1];
      nav2 = S.matSol[ber2][camada2];

      if (nav1 != nav2 && (nav1 != -1 || nav2 != -1) && (matTemAte_[ber1][nav2] != 0 && matTemAte_[ber2][nav1] != 0))
      {
         teste = false;
      }
   }

   S.matSol[ber1][camada1] = nav2;
   S.matSol[ber2][camada2] = nav1;

   S.vetIdBerNav[nav2] = ber1;
   S.vetIdBerNav[nav1] = ber2;

   if (att)
   {
      if (ber1 < ber2)
      {
         ajustar_hor_pos_berco(S, ber1);
         ajustar_hor_pos_berco(S, ber2);
      }
      else
      {
         ajustar_hor_pos_berco(S, ber2);
         ajustar_hor_pos_berco(S, ber1);
      }
   }

   calc_fo(S);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void gerarViz2(Solucao &S)
{ // Troca somente um navio de berco aleatório
   int nav1, ber1, ber2, camada1, teste = true;

   while (teste)
   {
      ber1 = rand() % (numBer_);

      if (S.vetQtdNavBer[ber1] > 0)
      {
         camada1 = (rand() % (S.vetQtdNavBer[ber1]));
      }
      else
      {
         while (S.vetQtdNavBer[ber1] == 0)
         {
            ber1 = rand() % (numBer_);
         }
         camada1 = (rand() % (S.vetQtdNavBer[ber1]));
      }

      nav1 = S.matSol[ber1][camada1];

      ber2 = rand() % (numBer_); // berco destino

      if (matTemAte_[ber2][nav1] != 0)
      {
         teste = false;
      }
   }

   for (int z = camada1; z < S.vetQtdNavBer[ber1]; z++)
   {
      S.matSol[ber1][z] = S.matSol[ber1][z + 1];
   }
   S.vetQtdNavBer[ber1]--;

   S.matSol[ber2][S.vetQtdNavBer[ber2]] = nav1;
   S.vetQtdNavBer[ber2]++;

   S.vetIdBerNav[nav1] = ber2;
   if (ber1 < ber2)
   {
      ajustar_hor_pos_berco(S, ber1);
      ajustar_hor_pos_berco(S, ber2);
   }
   else
   {
      ajustar_hor_pos_berco(S, ber2);
      ajustar_hor_pos_berco(S, ber1);
   }

   calc_fo(S);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void gerarViz3(Solucao &S)
{ // troca dois navios numBer_ vezes sucessivamente

   int nav1, nav2, ber1, ber2, camada1, camada2, teste = true, roda = 0;

   while (numBer_ > roda)
   {

      gerarViz1(S, false);
      roda++;
   }

   inicializar_hor_pos_navios(S);
   calc_fo(S);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void criar_solucao_por_tamanho(Solucao &s, float alpha)
{
   int nav, ber, aux;
   memset(&s.vetQtdNavBer, 0, sizeof(s.vetQtdNavBer));
   memset(&s.matSol, -1, sizeof(s.matSol));

   s.maiorQtdNavBer = 0;
   for (int i = 0; i < numNav_; i++)
   {
      nav = vetIndNavOrd_[i];
      ber = verificaMenorTempoBer(s, nav, alpha);
      if (matTemAte_[ber][nav] == 0)
      {
         aux = verificaMenorTempoBer(s, nav, alpha);
         while (matTemAte_[aux][nav] == 0)
         {
            aux = verificaMenorTempoBer(s, nav, alpha);
         }

         s.matSol[aux][s.vetQtdNavBer[aux]] = nav;
         s.vetIdBerNav[nav] = aux;
         s.vetQtdNavBer[aux]++;
      }
      else
      {
         s.matSol[ber][s.vetQtdNavBer[ber]] = nav;
         s.vetIdBerNav[nav] = ber;
         s.vetQtdNavBer[ber]++;
      }

      inicializar_hor_pos_navios(s);
   }
   for (int i = 0; i < numBer_; i++)
      if (s.vetQtdNavBer[i] >= s.maiorQtdNavBer)
         s.maiorQtdNavBer = s.vetQtdNavBer[i];
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void descobre_tam_total_dos_bercos()
{
   for (int i = 0; i < numBer_; i++)
   {

      vetTamTotalBer_[i] = vetTamLEBer_[i] + vetTamLDBer_[i];
      if (i != 0 && vetTamLDBer_[i - 1] != 0)
      {
         vetTamTotalBer_[i] += vetTamLDBer_[i - 1];
      }
      else if (i != numBer_ && vetTamLEBer_[i + 1] != 0)
      {
         vetTamTotalBer_[i] += vetTamLEBer_[i + 1];
      }

      vetIdBerOrdTamTotal_[i] = i;  // somente para preencher o vetor com os ids
      vetIdBercoMelhorTemp_[i] = i; // somente para preencher o vetor com os ids
   }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int verificaMenorTempoBer(Solucao &s, int nav, float alpha)
{

   int n = numBer_; // para não modificar o valor de numBer_
   int auxId, auxTam, vetAuxTemp[MAX_BER], vetAuxIdBerco[MAX_BER];
   memcpy(&vetAuxTemp, &s.vetHorAlocProxNav, sizeof(s.vetHorAlocProxNav));
   memcpy(&vetAuxIdBerco, &vetIdBercoMelhorTemp_, sizeof(vetIdBercoMelhorTemp_));

   for (int i = 0; i < (n - 1); i++)
   {
      for (int j = i; j < (n); j++)
      {

         if (matTemAte_[vetAuxIdBerco[i]][nav] == 0)
         {
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
         else if (matTemAte_[vetAuxIdBerco[j]][nav] == 0)
         {
            auxId = vetAuxIdBerco[n - 1];
            vetAuxIdBerco[n - 1] = vetAuxIdBerco[j];
            vetAuxIdBerco[j] = auxId;

            auxTam = vetAuxTemp[n - 1];
            vetAuxTemp[n - 1] = vetAuxTemp[j];
            vetAuxTemp[j] = auxTam;
            n--;
            j--;
         }
         else if (
             vetAuxTemp[i] + matTemAte_[vetAuxIdBerco[i]][nav] > vetAuxTemp[j] + matTemAte_[vetAuxIdBerco[j]][nav])
         {
            auxId = vetAuxIdBerco[i];
            vetAuxIdBerco[i] = vetAuxIdBerco[j];
            vetAuxIdBerco[j] = auxId;

            auxTam = vetAuxTemp[i];
            vetAuxTemp[i] = vetAuxTemp[j];
            vetAuxTemp[j] = auxTam;
         }
      }
   }

   int porcentagemBercos = (int)floor(numBer_ * alpha);
   if (porcentagemBercos == 0)
   {
      porcentagemBercos += 1;
   }

   return vetAuxIdBerco[rand() % porcentagemBercos];
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void escrever_solucao(Solucao &s, char *arq)
{
   FILE *f;
   if (strcmp(arq, "") == 0)
      f = stdout;
   else
      f = fopen(arq, "w");
   fprintf(f, "FO: %d\n", s.funObj);
   fprintf(f, "Tempo de servico: %d\n", s.temSer);
   fprintf(f, "--------------------------\n");
   fprintf(f, "Navio\tBerco\tTempo\tPosicao\n");
   for (int i = 0; i < numNav_; i++)
      fprintf(f, "%d\t%d\t%d\t%d\n", i + 1, s.vetIdBerNav[i] + 1, s.vetHorAtrNav[i], s.vetPosAtrNav[i]);
   fprintf(f, "\nMATRIZ DE SOLUCAO\n");
   for (int i = 0; i < numBer_; i++)
   {
      fprintf(f, "B%d: ", i + 1);
      for (int j = 0; j < s.vetQtdNavBer[i]; j++)
         fprintf(f, "%d  ", s.matSol[i][j] + 1);
      fprintf(f, "\n");
   }
   if (strcmp(arq, "") != 0)
      fclose(f);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ler_instancia(char *arq)
{
   FILE *f = fopen(arq, "r");
   fscanf(f, "%d\t%d\n", &numNav_, &numBer_);
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

//------------------------------------------------------------------------------
void ler_instancias_TCC(char *arq, int Inst, int Vez, int instGroup)
{

   switch (instGroup)
   {
   case 1: // trabalho de metodos
      sprintf(arq, ".//instancias//%s%d.txt", INST, Inst);
      ler_instancia(arq);
      break;
   case 2: // instancias moccia
      if (Inst < 10)
      {
         sprintf(arq, ".//instancias_moccia//%s0%d", INST_MOCCIA, Inst);
      }
      else
      {
         sprintf(arq, ".//instancias_moccia//%s%d", INST_MOCCIA, Inst);
      }
      ler_instancia_moccia(arq);
      break;
   case 3: // instancias continuo - não concluido
      sprintf(arq, ".//inst-continuo//%s%d.txt", INST_CONTINUO, Inst);
      break;
   default:
      break;
   }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void escrever_instancias_TCC(char *arq, int Inst, int Vez, int funObj, int instGroup, float alpha, int qtd)
{

   switch (instGroup)
   {
   case 1: // trabalho de metodos
      sprintf(arq, ".//instancias//%s%d-%d-%d.sol", INST, Inst, Vez, funObj);
      break;
   case 2: // instancias moccia
      // sprintf(arq, ".//instancias_moccia_result//%s%d-%d-%d.sol", INST, Inst, Vez, funObj);
      sprintf(arq, ".//moccia_final_result//%s%d-%d-%d-%.2f-%d.sol", INST, Inst, Vez, funObj, alpha, qtd);
      break;
   case 3: // instancias continuo - não concluido
      sprintf(arq, ".//instancias_moccia_result//%s%d-%d-%d.sol", INST, Inst, Vez, funObj);
      break;
   default:
      break;
   }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void escrever_instancias_arquivo_TCC(int Inst, int Vez, int funObj, double time)
{

   FILE *file = fopen(".//moccia_final_result/resultados.csv", "a");  // Abre o arquivo no modo de adição (append)
   if (file == NULL)
   {
      perror("Erro ao abrir o arquivo");
      return;
   }

   fprintf(file, "%d,%d,%d,%.6f\n", Inst, Vez, funObj, time);

   fclose(file); // Fecha o arquivo
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ler_instancia_moccia(char *arq)
{
   FILE *f = fopen(arq, "r");
   if (!f)
   {
      perror("Erro ao abrir o arquivo");
      exit(EXIT_FAILURE);
   }

   fscanf(f, "%d %d", &numNav_, &numBer_);

   for (int i = 0; i < numBer_; i++)
      for (int j = 0; j < numNav_; j++)
         fscanf(f, "%d", &matTemAte_[i][j]);

   for (int i = 0; i < numBer_; i++)
      fscanf(f, "%d %d %*d", &vetHorAbeBer_[i], &vetHorFecBer_[i]);

   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetHorCheNav_[i]);

   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetHorMaxNav_[i]);

   for (int i = 0; i < numBer_; i++)
      fscanf(f, "%d %d %*d %*d", &vetTamLEBer_[i], &vetTamLDBer_[i]);

   for (int i = 0; i < numNav_; i++)
      fscanf(f, "%d", &vetTamNav_[i]);

   fclose(f);
}