#ifndef cbapHPP
#define cbapHPP

#define MAX_BER 20
#define MAX_NAV 100

// ------------ Estrutura de dados
typedef struct tSolucao {
   int vetIdBerNav[MAX_NAV];
   int vetPosAtrNav[MAX_NAV];
   int vetPosFinNav[MAX_NAV];
   int vetHorAtrNav[MAX_NAV];
   int vetHorSaiNav[MAX_NAV];
   int vetHorAlocProxNav[MAX_BER]; // tempo de alocação para o proximo navio
   //---
   int vioHorMaxNav;
   int vioHorFecBer;
   int temSer;
   int funObj;
   //---
   int vetQtdNavBer[MAX_BER];
   int matSol[MAX_BER][MAX_NAV];

   // novo

   int maiorQtdNavBer;
}Solucao;

// ------------ Dados de entrada
int numBer_;                      // n�mero de ber�os dispon�veis
int numNav_;                      // n�mero de navios a serem atendidos
int vetHorAbeBer_[MAX_BER];       // vetor com o hor�rio de abertura de cada ber�o
int vetHorFecBer_[MAX_BER];       // vetor com o hor�rio de fechamento de cada ber�o
int vetTamLDBer_[MAX_BER];        // vetor com o tamanho do lado direito de cada ber�o
int vetTamLEBer_[MAX_BER];        // vetor com o tamanho do lado esquerdo de cada ber�o
int vetHorCheNav_[MAX_NAV];       // vetor com o hor�rio de chegada de cada navio
int vetHorMaxNav_[MAX_NAV];       // vetor com o hor�rio m�ximo de sa�da de cada navio
int vetTamNav_[MAX_NAV];          // vetor com o tamanho de cada navio
int matTemAte_[MAX_BER][MAX_NAV]; // matriz com o tempo de atendimento de cada navio (coluna) em cada ber�o (linha)

// ------------ Vari�veis auxiliares
int vetIndNavOrd_[MAX_NAV];
int vetPosIniBer_[MAX_BER];
int vetPosFinBer_[MAX_BER];
int vetTamTotalBer_[MAX_BER]; // tamanho total que x berco pode ocupar
int vetIdBerOrdTamTotal_[MAX_BER]; // id do berco ordenado pelo seu tamanho crescente
// int vetTempAtualBer_[MAX_BER]; // tempo atual de atendimento do berco
int vetIdBercoMelhorTemp_[MAX_BER]; // vetor com os ids do berco que com o barco atual ocupa o menor tempo


//********************************* M�TODOS **********************************
void ajustar_hor_pos_berco(Solucao &s, const int &ber);
void calc_fo(Solucao &s);
void inicializar_hor_pos_navios(Solucao &s);
bool navios_sobrepostos(Solucao &s, const int &n1, const int &n2);
void verificar_solucao(Solucao &s, bool indicador);
void escrever_solucao(Solucao &s, char *arq);
void clonar_solucao(Solucao &o, Solucao &c);
void ordenar_navios(int *vetInd, int *vetVal);
void atualizar_dimensoes_bercos();
void ler_instancia(char *arq);
void criar_solucao_por_tamanho(Solucao &s, float alpha);
void descobre_tam_total_dos_bercos();
int verificaMenorTempoBer(Solucao &s, int nav, float alpha);

void iniciaGrasp(int Inst, int Vez,  double ITmaxTempo, float alpha, int qtd, int instGroup);
void ler_instancia_moccia(char *arq);
void ler_instancias_TCC(char *arq, int Inst, int Vez, int instGroup);
void escrever_instancias_TCC(char *arq, int Inst, int Vez, int funObj, int instGroup, float alpha, int qtd);
void escrever_instancias_arquivo_TCC(int Inst, int Vez, int funObj, double time);
void grasp(Solucao &s, double ITmaxTempo, float alpha, int qtd, double &bestFOtime);
void VNS(Solucao &S, int qViz, int qtd);
void gerarViz1(Solucao &S,bool att); // troca dois navios aleatorios entre si
void gerarViz2(Solucao &S); // troca um navio aleatorio de berço
void gerarViz3(Solucao &S); // troca dois navios aleatorios entre numBer_ vezes

#endif
double tempMelhorSol;