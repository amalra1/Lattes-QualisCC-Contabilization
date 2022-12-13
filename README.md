# O que o projeto faz?

  Contabilização da Classificação da Produção Científica no Lattes usando o Qualis-CC,
feito para a disciplina de Programação 2, com o professor David Menotti:
http://wiki.inf.ufpr.br/maziero/doku.php?id=prog2:lattes-qualis

## Como ele funciona?

  **-> Fonte de chamada:**
  
  ```bash
  
  ./lattes -d [nome_do_diretório] -c qualis_conf.txt -p qualis_periodicos.txt
  
  ```
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
  --> Sumarizada de periódicos de acordo com os níveis;  
  --> Sumarizada de conferências de acordo com os níveis;  
  --> Sumarizada das produções de cada pesquisador de acordo com os níveis;  
  --> Sumarizada das produções de acordo com o ano de suas realizações;  
  --> Sumarizada de todas as produções classificadas em nível C;  
  --> Sumarizada de todas as produções Não Classificadas.  
  

## Informações adicionais

  O programa utiliza uma função de distância de edição, extraída do link:
https://github.com/wooorm/levenshtein.c
