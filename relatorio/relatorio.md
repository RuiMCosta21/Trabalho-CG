<h1 align="center">Universidade do Minho</h1>
<h1 align="center">Computação Gráfica - 2ª Fase</h1>
 
### Hélder Miguel Cunha Alves - a104609
### Diogo Luı́s Barros Costa - a100751
### Rui Mário da Silva Costa - a107316


## 1. Representação das transformações

&nbsp;&nbsp;&nbsp;&nbsp;Representamos em memória as transformações de cada grupo acumulando-as numa única matriz. Para este efeito implementamos uma tradução direta entre os valores das coordenadas das transformções lidos no ficheiro de configuração e as suas representações matriciais (como lecionado nas aulas). Esta abordagem permite aplicá-las numa única instrução no momento de renderização, bem como poupar na memória utilizada.

## 2. Atualização do estado interno da aplicação engine

&nbsp;&nbsp;&nbsp;&nbsp;De maneira a acomodar a informação adicionada ao ficheiro de configuração, atualizamos a nossa aplicação para tratar cada grupo como um objeto individual.  
&nbsp;&nbsp;&nbsp;&nbsp;Um objeto Grupo é constituído pelos os seguintes parâmetros:
- Uma matriz 4x4 que representa as suas transformações (translação, rotação e escala);
- Um vetor com os modelos;
- Um vetor de apontadores para Grupos filhos

&nbsp;&nbsp;&nbsp;&nbsp;Desta estrutura resulta uma *Rose Tree* que facilita o *bubble down* da aplicação das transformações.

<figure>
  <img src="RoseTreeEx.png" alt="Rose Tree">
  <figcaption align="center">Figura 1: Representação da hierarquia de grupos <em>(imagem gerada com o auxílio de IA)</em> </figcaption>
</figure>

## 3. Renderização da cena

&nbsp;&nbsp;&nbsp;&nbsp;A renderização da cena é feita de maneira recursiva percorrendo a árvore de grupos usando um algoritmo *Depth-First*. A herança das transformações de pai para filhos é implementada tomando proveito da stack de matrizes do OpenGL: ao fazer push da matriz do pai, podemos lhe somar as transformações dos filhos e aplicá-la. Fazemos pop quando um grupo não tem filhos, o que restaura a matriz anterior.

## 4. Nova primitiva *Disk*

&nbsp;&nbsp;&nbsp;&nbsp;Para representar o anel de Saturno na *demo scene* do Sistema Solar, recorremos à criação de uma nova primitiva *Disk*, que consiste num plano raso em forma de anel. Toma por parâmetros o raio interior, o raio exterior e o número de fatias.  
&nbsp;&nbsp;&nbsp;&nbsp;O número de fatias (*n*) serve para calcular o incremento angular (*Δθ* ), dividindo uma volta completa pelo número de fatias:
<p align="center"><em>Δθ = 2π/n</em></p>

&nbsp;&nbsp;&nbsp;&nbsp;​Para cada passo *i* (onde 0 ≤ *i* ≤ *n*), o ângulo atual é:
<p align="center"><em>θ<sub>i</sub> ​ = i ⋅ Δθ</em></p>

&nbsp;&nbsp;&nbsp;&nbsp;Podemos assim representar o conjunto de vértices pertencentes às duas circunferências que definem o disco como
<p align="center"><em>P(x = r<sub>inner</sub> ⋅ cos(θi), y = 0, z = r<sub>inner</sub> ⋅ sin(θi))</em></p>
<p align="center"><em>P(x = r<sub>outer</sub> ⋅ cos(θi), y = 0, z = r<sub>outer</sub> ⋅ sin(θi))</em></p>  


&nbsp;&nbsp;&nbsp;&nbsp;Um segmento de anel é um trapézio definido por quatro pontos: dois na fatia atual (i) e dois na fatia seguinte (i+1). Para renderização, este quadrilátero é dividido em dois triângulos:

<p align="center">Triângulo A: {P<sub>in</sub>​(i),P<sub>out</sub>​(i),P<sub>in</sub>​(i+1)}</p>
<p align="center">Triângulo B: {P<sub>out</sub>​(i),P<sub>out</sub>​(i+1),P<sub>in</sub>(i+1)}</p>


## Bibliografia

- https://www.vaticanobservatory.org/sacred-space-astronomy/size-and-distance-models/ para as medidas do sistema solar


