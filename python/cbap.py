import random
import plotly.graph_objects as go
import plotly.io as pio
pio.renderers.default='browser'

inst_id = 1
inst_vez = 1
inst_fo = 2117
alpha = "0.10"
qtd_vns = "0"

# LINUX
# arq_instancia = '..//instancias/inst' + str(inst_id) + '.txt'
# arq_solucao = '..//boas-inst/inst' + str(inst_id) + '-' + str(inst_vez) + '-' + str(inst_fo) + '.sol'
# arq_solucao = '..//instancias/inst' + str(inst_id) + '-' + str(inst_vez) + '-' + str(inst_fo) + '.sol'

# WINDOWS
# arq_instancia = '.\instancias\inst' + str(inst_id) + '.txt'
arq_instancia = '.\instancias_moccia\i0' + str(inst_id) + '' # moccia
# arq_solucao = '..//boas-inst/inst' + str(inst_id) + '-' + str(inst_vez) + '-' + str(inst_fo) + '.sol'
# arq_solucao = '.\instancias\inst' + str(inst_id) + '-' + str(inst_vez) + '-' + str(inst_fo) + '.sol'
arq_solucao = '.\parametros_moccia\inst' + str(inst_id) + '-' + str(inst_vez) + '-' + str(inst_fo) + '-' + str(alpha) + '-' + str(qtd_vns) + '.sol' # moccia

#-----------------------------------------------------------------------------------------------------------------------
def ler_instancia(arq_inst):
    with open(arq_inst) as f:
        arq = f.readlines()
    #---
    aux = arq[0]
    aux = aux.split()
    navios = int(aux[0])
    bercos = int(aux[1])
    #---
    vet_tamanho_N = [0] * navios
    aux = arq[2]
    aux = aux.split()
    for i in range(navios):
        vet_tamanho_N[i] = int(aux[i])
    #---
    vet_tem_che_N = [0] * navios
    aux = arq[4]
    aux = aux.split()
    for i in range(navios):
        vet_tem_che_N[i] = int(aux[i])
    #---
    vet_tem_max_N = [0] * navios
    aux = arq[6]
    aux = aux.split()
    for i in range(navios):
        vet_tem_max_N[i] = int(aux[i])
    #---
    vet_tem_abe_B = [0] * bercos
    vet_tem_max_B = [0] * bercos
    for i in range(bercos):
        aux = arq[8+i]
        aux = aux.split()
        vet_tem_abe_B[i] = int(aux[0])
        vet_tem_max_B[i] = int(aux[1])
    #---
    vet_tam_viz_esq_B = [0] * bercos
    vet_tam_viz_dir_B = [0] * bercos
    for i in range(bercos):
        aux = arq[8+bercos+1+i]
        aux = aux.split()
        vet_tam_viz_esq_B[i] = int(aux[0])
        vet_tam_viz_dir_B[i] = int(aux[1])
    #---
    mat_tem_ate = [[0] * (navios) for i in range(bercos)]        
    for i in range(bercos):
        aux = arq[8+bercos+1+bercos+1+i]
        aux = aux.split()
        for j in range(navios):
            mat_tem_ate[i][j] = int(aux[j])
    #---    
    return navios, bercos, vet_tamanho_N, vet_tem_che_N, vet_tem_max_N, vet_tem_abe_B, vet_tem_max_B, vet_tam_viz_esq_B, vet_tam_viz_dir_B, mat_tem_ate
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
def ler_instancia_moccia(arq_inst):
    with open(arq_inst) as f:
        arq = f.readlines()
    
    # Lendo o número de navios (n) e o número de berços (m)
    aux = arq[0].split()
    navios = int(aux[0])
    bercos = int(aux[1])
    
    # Lendo os tempos de processamento dos navios em cada berço (m x n matriz)
    mat_tem_ate = []
    for i in range(bercos):
        aux = list(map(int, arq[1 + i].split()))
        mat_tem_ate.append(aux)
    
    # Lendo os tempos de abertura e fechamento dos berços
    vet_tem_abe_B = []
    vet_tem_max_B = []
    for i in range(bercos):
        aux = list(map(int, arq[1 + bercos + i].split()))
        vet_tem_abe_B.append(aux[0])  # Início da disponibilidade do berço
        vet_tem_max_B.append(aux[1])  # Fim da disponibilidade do berço
    
    # Lendo os tempos de chegada dos navios
    vet_tem_che_N = list(map(int, arq[1 + 2 * bercos].split()))
    
    # Lendo os tempos máximos de permanência dos navios
    vet_tem_max_N = list(map(int, arq[2 + 2 * bercos].split()))
    
    # Lendo os dados dos berços (tamanho lateral e vizinhos)
    vet_tam_viz_esq_B = []
    vet_tam_viz_dir_B = []
    for i in range(bercos):
        aux = list(map(int, arq[3 + 2 * bercos + i].split()))
        vet_tam_viz_esq_B.append(aux[0])  # Tamanho do lado esquerdo
        vet_tam_viz_dir_B.append(aux[1])  # Tamanho do lado direito
        # Os valores aux[2] e aux[3] representam os vizinhos, mas não são usados aqui
    
    # Lendo os tamanhos dos navios
    vet_tamanho_N = list(map(int, arq[3 + 3 * bercos].split()))

    return navios, bercos, vet_tamanho_N, vet_tem_che_N, vet_tem_max_N, vet_tem_abe_B, vet_tem_max_B, vet_tam_viz_esq_B, vet_tam_viz_dir_B, mat_tem_ate
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
def ler_solucao(arq_sol, navios):
    with open(arq_sol) as f:
        arq = f.readlines()
    #---
    fo = arq[0]
    fo = fo.split()
    fo = fo[1]
    #---
    tempo_servico = arq[1]
    tempo_servico = tempo_servico.split()
    tempo_servico = tempo_servico[3]
    #---
    solucao = []
    for i in range(navios):
        aux = arq[i+4].split()
        solucao.append([int(aux[0]), int(aux[1]), int(aux[2]), int(aux[3])])
    return fo, tempo_servico, solucao
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# PROGRAMA PRINCIPAL
navios, bercos, vet_tamanho_N, vet_tem_che_N, vet_tem_max_N, vet_tem_abe_B, vet_tem_max_B, vet_tam_viz_esq_B, vet_tam_viz_dir_B, mat_tem_ate = ler_instancia_moccia(arq_instancia)
print('\n----------------------------')
print('INSTÂNCIA')
print('----------------------------')
print(f'Navios................................: {navios}')
print(f'Berços................................: {bercos}')
print(f'Tamanho dos navios....................: {vet_tamanho_N}')
print(f'Tempo chegada dos navios..............: {vet_tem_che_N}')
print(f'Tempo máximo dos navios...............: {vet_tem_max_N}')
print(f'Tempo de abertura dos berços..........: {vet_tem_abe_B}')
print(f'Temop máximo dos berços...............: {vet_tem_max_B}')
print(f'Tamanho vizinho esquerdo dos berços...: {vet_tam_viz_esq_B}')
print(f'Tamanho vizinho direito dos berços....: {vet_tam_viz_dir_B}')
print('Matriz com o tempo de atendimento dos navios em cada berço:')
for i in range(bercos):
    print(f'{mat_tem_ate[i]}  ')
#---
fo, tempo_servico, solucao = ler_solucao(arq_solucao, navios)
print('\n\n----------------------------')
print('SOLUÇÃO ARQUIVO')
print('----------------------------')
print(f'FO.........................: {fo}')
print(f'Tempo de serviço...........: {tempo_servico}\n')
print('Solucao ([Navio, Berço, Tempo, Posição]):')
for i in range(navios):
    print(solucao[i])
#---
# Criar o gráfico com a solução
cores = []
for j in range(navios):
    cores.append("#"+''.join([random.choice('0123456789ABCDEF') for i in range(6)]))
fig = go.Figure()
#---
pi = 0
for i in range(bercos):    
    pf = pi + vet_tam_viz_esq_B[i] + vet_tam_viz_dir_B[i]
    fig.add_vrect(x0=pi, x1=pf, annotation_text=f"B{i+1}", annotation_position="bottom left",)
    if i != 0 and i != bercos-1:  
        if vet_tam_viz_esq_B[i] == 0 and vet_tam_viz_dir_B[i+1] == 0:
            fig.add_vline(x=pi, line_width=3, line_dash="dash", line_color='red')
        else:
            fig.add_vline(x=pf - vet_tam_viz_dir_B[i], line_width=2, line_dash="dot")        
    pi = pf
#---
for i in range(navios):
    dados = solucao[i]
    navio = dados[0]
    berco = dados[1]
    t = dados[2]
    p = dados[3]
    tamanho = vet_tamanho_N[navio-1]
    tempo = mat_tem_ate[berco-1][navio-1]
    fig.add_scatter(x=[p, p, p+tamanho, p+tamanho], y=[t, t+tempo, t+tempo, t], fill='toself',
                    fillcolor=cores[i], opacity=1, name=f'Navio: {navio}<br>Berço: {berco}<br>P: {p}<br>T: {t}<br>Tempo: {tempo}<br>Tamanho: {tamanho}', 
                    hoverinfo="text", line=dict(color=cores[i], width=1), marker=dict(size=1, line=dict(width=0, color=cores[i])))
#---
fig.update_layout(title=f'Navios: {navios}   Berços: {bercos}   Tempo de serviço: {tempo_servico}', showlegend=False, template='plotly_dark')
fig.update_xaxes(title="Espaço (berços)", showgrid=False)
fig.update_yaxes(title="Tempo", showgrid=False)
fig.show()