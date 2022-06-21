# Lista de Execícios de Programação Concorrente e distribuída

questões resolvidas: [index.md](docs/index.md)

## mkdocs
documentação criada com  [squidfunk.github.io/mkdocs-material/](https://squidfunk.github.io/mkdocs-material/)

### instalação de dependências

É aconselhável criar um ambiente virtual antes instalar as dependências
como
```shell
python -mvenv .env
source .env/bin/activate
```

para instalar as dependências:
```shell
pip install -r requirements.txt
```

### Visualizar a documentação

```shell
mkdocs serve
```


### Por a documentação em  Programação
Já foi configurado o CI do github [ci.yml](..github/workflows/ci.yml)
para fazer o deploy da documentação assim que for atualizado o branch
main, por tanto não é aconselhável fazer o deploy manualmente.
Mas caso necessário,
para gerar a página no github:
```shell
mkdocs gh-deploy --force
```
a documentação pode ser visualizada na página:  
[samuel-cavalcanti.github.io/parallel_programming_ufrn/](https://samuel-cavalcanti.github.io/parallel_programming_ufrn/)

### Gerando o PDF

Para gerar o Pdf foi utilizado o plugin **with-pdf**. Informações
sobre o plugin pode ser encontrado no repositório do projeto [github.com/orzih/mkdocs-with-pdf](https://github.com/orzih/mkdocs-with-pdf)

para compilar a documentação e gerar o pdf, basta compilar a aplicação

```shell
mkdocs build
```

o pdf gerado será encontrado no diretório **site/assets/pdf/**.
O pdf da documentação atual pode ser pode ser baixado ou visualizado no link [amuel-cavalcanti.github.io/parallel_programming_ufrn/assets/pdf/document.pdf](https://samuel-cavalcanti.github.io/parallel_programming_ufrn/assets/pdf/document.pdf)
