# Collision Simulator in SDL

[Português](#português) | [English](#english)

## Screenshots


<div align="center">
  [Simulation Preview](assets/Screenshot.png)
</div>

---

<a id="português"></a>
## Português

Este é um simulador de física 2D desenvolvido em C utilizando a biblioteca SDL2. O projeto simula colisões elásticas entre bolas, com suporte a gravidade, centro de massa interativo e efeitos sonoros.

### Funcionalidades

*   **Simulação de Física:** Colisões elásticas entre bolas e com as bordas.
*   **Gravidade:** Alterne a gravidade para ver como ela afeta os objetos.
*   **Centro de Massa:** Ative um centro de massa gravitacional que atrai as bolas.
*   **Controle de Quantidade:** Adicione ou remova bolas dinamicamente durante a simulação.
*   **Efeitos Visuais:** Visualização de vetores de movimento (opcional).
*   **Áudio:** Efeitos sonoros de colisão.
*   **Menu Inicial:** Interface gráfica simples para iniciar a simulação.

### Pré-requisitos

Para compilar e executar este projeto, você precisará das bibliotecas de desenvolvimento do SDL2 instaladas no seu sistema.

#### Linux (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Como Compilar e Executar

O projeto utiliza um `Makefile` para facilitar a compilação.

#### Compilar
Para apenas compilar o projeto:
```bash
make
```

#### Compilar e Executar
Para compilar e iniciar o simulador imediatamente:
```bash
make run
```

#### Limpar Arquivos Temporários
Para remover os arquivos objeto e o binário compilado:
```bash
make clean
```

### Controles

Durante a execução do simulador, utilize as seguintes teclas para interagir:

| Tecla | Ação |
| :--- | :--- |
| `ESC` | Sair do simulador |
| `P` | Pausar / Despausar a simulação |
| `S` | Embaralhar (Shuffle) as bolas |
| `G` | Ativar / Desativar Gravidade |
| `C` | Ativar / Desativar Centro de Massa |
| `V` | Mostrar / Ocultar Vetores (Debug) |
| `M` | Mutar / Desmutar Sons |
| `+` (ou Numpad `+`) | Adicionar uma nova bola aleatória |
| `-` (ou Numpad `-`) | Remover a última bola |

---

<a id="english"></a>
## English

This is a 2D physics simulator developed in C using the SDL2 library. It simulates elastic collisions between balls, featuring gravity support, an interactive center of mass, and sound effects.

### Features

*   **Physics Simulation:** Elastic collisions between balls and boundaries.
*   **Gravity:** Toggle gravity to observe its effect on objects.
*   **Center of Mass:** Activate a gravitational center of mass that attracts the balls.
*   **Quantity Control:** Dynamically add or remove balls during the simulation.
*   **Visual Effects:** Motion vector visualization (debug mode).
*   **Audio:** Collision sound effects.
*   **Start Menu:** Simple graphical interface to start the simulation.

### Prerequisites

To build and run this project, you will need the SDL2 development libraries installed on your system.

#### Linux (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### How to Build and Run

The project uses a `Makefile` to streamline the build process.

#### Build
To only build the project:
```bash
make
```

#### Build and Run
To build and immediately start the simulator:
```bash
make run
```

#### Clean Temporary Files
To remove object files and the compiled binary:
```bash
make clean
```

### Controls

Use the following keys to interact with the simulator while running:

| Key | Action |
| :--- | :--- |
| `ESC` | Exit the simulator |
| `P` | Pause / Unpause simulation |
| `S` | Shuffle balls |
| `G` | Toggle Gravity |
| `C` | Toggle Center of Mass |
| `V` | Show / Hide Vectors (Debug) |
| `M` | Mute / Unmute Sounds |
| `+` (or Numpad `+`) | Add a random ball |
| `-` (or Numpad `-`) | Remove the last ball |

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
