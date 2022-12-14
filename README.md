# O que o programa faz?

  Contabilização da Classificação da Produção Científica no Lattes usando o Qualis-CC,
feito durante a disciplina de [Programação 2](http://wiki.inf.ufpr.br/maziero/doku.php?id=prog2:lattes-qualis).  

## Utilização e funcionamento
  
* **Instalação**  
  
  Garanta que todos os arquivos deste repositório estejam juntos no mesmo lugar, fácil com o comando do git:
  
  ```
  git clone git@github.com:amalra1/ProjLattes.git
  ```
  > Aviso, este comando só funcionará se você já tiver uma chave SSH integrada ao GitHub, se não possui: [clique aqui](https://docs.github.com/pt/authentication/connecting-to-github-with-ssh)
  
  Após isso, compile com o comando no terminal, `make`
  
  <br />
  
* **Fonte de chamada:**  
  
  ```bash
  
  ~/ ./lattes -d [nome_do_diretório] -c qualis_conf.txt -p qualis_periodicos.txt
  
  ```
  <br />
  O diretório em questão precisa conter apenas os arquivos .XML gerados dos currículos lattes 
  dos pesquisadores, por exemplo:
  
  ```bash
  
  ~/ cd algoritmos
  ~/algoritmos ls
  
  curriculoPesq1.xml
  curriculoPesq2.xml
  curriculoPesq3.xml
  
  ```  
  Nestes arquivos, coleta o nome dos **periódicos** em que o pesquisador publicou,  
  e também o nome dos eventos(**conferências**) que participou;  
    
  Depois de coletados, cataloga os títulos de acordo com seus níveis na QUALIS  
  **{A1, A2, A3, A4, B1, B2, B3, B4, C}**, e também '**C-**', nivel criado para se referir a   
  aqueles que não estão na lista de classificação, portanto, sem classificação.  
    
  
  E por fim, imprime as informações da seguinte maneira:  
  > Sumarizada de periódicos de acordo com os níveis;  
  > Sumarizada de conferências de acordo com os níveis;  
  > Sumarizada das produções de cada pesquisador de acordo com os níveis;  
  > Sumarizada das produções de acordo com o ano de suas realizações;  
  > Sumarizada de todas as produções classificadas em nível C;  
  > Sumarizada de todas as produções Não Classificadas.  
  

## Informações adicionais

  O programa utiliza uma função de distância de edição, extraída do [link](https://github.com/wooorm/levenshtein.c).

## Mudanças previstas

 - [ ] Encontrar uma função de distância de edição melhor do que a atual,  
       ou melhorar o funcionamento da mesma;
       
 - [ ] Alterar a estrutura de dados **pesquisador** para uma mais eficiente;
 
 - [ ] Alterar o método de catalogação para um mais eficiente.

 - [ ] Alterar a fonte de chamada para receber o caminho dos arquivos ao 
       invés de seus nomes.
 
