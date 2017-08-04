# SGM #

Sistema de gerenciamento de menções.

### Dependências ###

* __g++ >= 4.8__
* __pdflatex__

### Instalação ###

Os passos a seguir descrevem o processo de instalação da ferramenta _sgm_.

0. Clone o repositório para uma pasta temporária em seu computador
0. Entre na pasta onde se encontra o repositório
0. Compile a ferramenta com o seguinte comando:
    `$ make compile`
0. Instale a ferramenta com o seguinte comando:
    `$ make install`

### Utilização do SGM ###

Após a instalação, será possível utilizar os comandos:

* `sgm`: mostrar os comandos disponíveis da ferramenta;
* `sgm init`: inicializar uma pasta do SGM;
* `sgm build`: gerar os arquivos e pastas de atividades avaliativas não encontradas e montar a tabela de notas em _latex_;
* `sgm genpdf`: executar o `sgm build` e gerar o PDF de menções.

### Exemplo de uso ###

O repositório contém uma pasta chamada __sample__ com um exemplo de utilização do sistema SGM.
