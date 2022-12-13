# O que o projeto faz?

  Contabilização da Classificação da Produção Científica no Lattes usando o Qualis-CC,
feito para a disciplina de Programação 2, com o professor David Menotti:
http://wiki.inf.ufpr.br/maziero/doku.php?id=prog2:lattes-qualis

# Como ele funciona?

  -> Fonte de chamada:
  
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

# Informações adicionais

  O programa utiliza uma função de distância de edição, extraída do link:
https://github.com/wooorm/levenshtein.c
