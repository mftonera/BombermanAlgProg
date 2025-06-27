
# Mini Bomberman — Algoritmos e Programação (UFRGS)

Este é um projeto desenvolvido como parte da disciplina **INF01202 - Algoritmos e Programação** na UFRGS. Ele é uma recriação do clássico **Bomberman**, utilizando a biblioteca gráfica **Raylib** e a linguagem C.

---

##  Funcionalidades principais

-  Movimentação do jogador em um mapa 2D.
- Colocação de bombas e explosão com tempo e alcance definidos.
- Inimigos com movimentação e colisão com o jogador.
- Blocos destrutíveis e indestrutíveis.
- Saída oculta que aparece após a destruição da parede destrutível.
- Power-ups: preenchimento do número de bombas e aumento do alcance da explosão.
- Editor de mapas com salvamento no formato `.txt`.
- Menu de pausa com opções.
- Carregamento e seleção de mapas personalizados pelo menu inicial.

---

## Como compilar

### Requisitos

- [Raylib](https://www.raylib.com/) instalada (v4.5 ou superior)
- Compilador C (MinGW no Windows, GCC no Linux/Mac)

### Comandos

Para compilar o jogo:

```bash
make
```

Para remover os arquivos compilados:

```bash
make clean
```

---

## Como jogar

- **Setas e WASD**: movimentação do jogador.
- **espaço**: planta uma bomba.
- **ESC**: abre o menu de pausa.
- **Menu inicial**:
  - **Novo jogo**: inicia um novo mapa.
  - **Carregar jogo**: carrega um save.
  - **Mapa personalizado**: permite escolher e jogar mapas salvos.
  - **Editor de mapas**: cria e salva novos mapas.

---

## Salvamento e Mapas

- O progresso pode ser salvo pelo menu de pausa.
- Mapas personalizados são do tipo `.txt` contendo:
  - `W` para tiles indestrutíveis
  - `B` para tiles destrutíveis
  - ` ` (espaço) para tiles livres

---

## Autores

Projeto desenvolvido por **Bruna Zamboni Lima**, **Eduardo Franceschina Nunes** e **Matheus Fracaro Tonera**  com base nas especificações da disciplina **INF01202 - Algoritmos e Programação (UFRGS)**.

---

## Créditos

Créditos para Andrew Hamel, que fez ser possível a continuidade do projeto com base nos arquivos base do Make e da pasta .vscode de um dos seus projetos anteriores.
