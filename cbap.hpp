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
int vetTemAtualBer_[MAX_BER]; // indica o tempo atual de disponibilidade do berço


//********************************* M�TODOS **********************************
void ajustar_hor_pos_berco(Solucao &s, const int &ber);
void calc_fo(Solucao &s);
void criar_solucao(Solucao &s);
void inicializar_hor_pos_navios(Solucao &s);
bool navios_sobrepostos(Solucao &s, const int &n1, const int &n2);
void verificar_solucao(Solucao &s);
void ler_solucao(Solucao &s, char *arq);
void escrever_solucao(Solucao &s, char *arq);
void clonar_solucao(Solucao &o, Solucao &c);
void ordenar_navios(int *vetInd, int *vetVal);
void testar_instancia(char *arq);
void atualizar_dimensoes_bercos();
void ler_instancia(char *arq);
void criar_solucao_por_tamanho(Solucao &s);
void descobre_tam_total_dos_bercos();
void ordernar_berco_asc();
int verifica_menor_tempo_atendimento_nav(int nav);

// novo
void testaVNS(int Inst, int Vez);
void ordenar_nav_hor_che(Solucao &s,int ber); // ordena os navios por ordem de chegada
void VNS(Solucao &S, double ITmaxTempo, int qViz, int qtd);
void heuBLPM(Solucao &S); // Heuristica primeira melhora
void gerarViz1(Solucao &S,bool att); // troca dois navios aleatorios entre si
void gerarViz2(Solucao &S); // troca um navio aleatorio de berço
void gerarViz3(Solucao &S); // troca dois navios aleatorios entre numBer_ vezes
void gerarViz4(Solucao &S); // troca um navio aleatorio de berço numBer_ vezes

#endif

// Pegar info do professor
double tempMelhorSol;