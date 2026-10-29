# ESP32 Remote Car

Projeto de um carrinho de controle remoto utilizando **ESP32 + ESP-IDF + Bluepad32**, com arquitetura modular para facilitar manutenção e evolução.

## Objetivos

- Controle via Bluetooth utilizando controles compatíveis com Bluepad32
- Arquitetura desacoplada entre entrada, lógica do veículo e hardware
- Fácil expansão para:
  - Luzes
  - Buzina
  - Servo de direção
  - Telemetria
  - OTA
  - Sensores
  - Monitoramento de bateria

---

# Tecnologias

- ESP32
- ESP-IDF v5.3.5
- Bluepad32 v4.2.0
- BTstack v1.6.2

---

# Estrutura do projeto

```
main/
│
├── bluetooth/
│   ├── bluetooth.c
│   └── bluetooth.h
│
├── board/
│   ├── board.c
│   └── board.h
│
├── config/
│   ├── config.c
│   └── config.h
│
├── drivers/
│   ├── horn.c
│   ├── horn.h
│   ├── lights.c
│   ├── lights.h
│   ├── motors.c
│   ├── motors.h
│   ├── pwm.c
│   └── pwm.h
│
├── gamepad/
│   ├── gamepad_button_state.c
│   ├── gamepad_button_state.h
│   ├── gamepad_logger.c
│   ├── gamepad_logger.h
│   ├── gamepad_mapper.c
│   └── gamepad_mapper.h
│   ├── gamepad.c
│   ├── gamepad.h
│
├── vehicle/
│   ├── vehicle_command.c
│   └── vehicle_command.h
│   ├── vehicle.c
│   ├── vehicle.h
│
├── failsafe.c
├── failsafe.h
├── main.c
├── util.c
└── util.h
```

---

# Arquitetura

```
                     Bluepad32
                         │
                         ▼
                  bluetooth/
                         │
                         ▼
                  gamepad_logger
                         │
                         ▼
                  gamepad_mapper
                         │
                         ▼
                  VehicleCommand
                         │
                         ▼
                vehicle_apply_command()
                         │
                         ▼
                  VehicleState
     ┌─────────────┬─────────────┬─────────────┐
     ▼             ▼             ▼             ▼
  Motors        Lights         Horn       (Futuro)
```

O projeto utiliza uma arquitetura baseada em estados.

- O controle não acessa diretamente o hardware.
- O hardware não conhece o controle Bluetooth.
- Toda comunicação passa por `VehicleCommand` e `VehicleState`.

---

## Organização

O projeto foi dividido em quatro camadas principais.

### Bluetooth

Responsável apenas pela comunicação com o controle Bluetooth utilizando Bluepad32.

Não possui conhecimento sobre motores, iluminação ou buzina.

### Gamepad

Converte os dados recebidos do controle em comandos de alto nível.

Exemplo:

- acelerar
- esterçar
- ligar seta
- trocar intensidade do farol

O resultado é um `VehicleCommand`.

### Vehicle

Responsável pela lógica do veículo.

Recebe um `VehicleCommand` e produz um `VehicleState`.

Exemplos:

- qual farol está ligado
- seta esquerda ativa
- buzina ligada
- velocidade
- direção

### Drivers

Responsáveis apenas pelo hardware.

Cada driver lê o `VehicleState` e controla seus respectivos GPIOs.

- motors
- servo
- lights
- horn

### Modos de direção

O modo é selecionado por `BOARD_DRIVE_TYPE` em `main/board/board.h`:

- `DRIVE_ACKERMANN`: motor 1 faz a tração e o servo no GPIO 26 faz a direção. O segundo motor da ponte H não é usado para esterçar.
- `DRIVE_DIFFERENTIAL`: motor 1 e motor 2 fazem os lados esquerdo e direito do carrinho. O servo não é inicializado nesse modo.

O servo usa um timer LEDC separado, a 50 Hz. O PWM da ponte H e do farol permanece em 20 kHz.

---

## Mapeamento do Controle
| Controle             | Função                       |
| -------------------- | ---------------------------- |
| Analógico esquerdo X | Direção                      |
| R1                   | Alterna seta esquerda        |
| L1                   | Alterna seta direita         |
| R2                   | Acelerar                     |
| L2                   | Freio / Ré                   |
| A                    | Buzina                       |
| B                    | Pisca-alerta                 |
| DPad ↑               | Aumenta intensidade do farol |
| DPad ↓               | Diminui intensidade do farol |

---

## Mapeamento do GPIO
| GPIO | Função               |
| ---- | -------------------- |
| GPIO | Função                                                   |
| ---- | -------------------------------------------------------- |
| 18   | Motor 1, entrada A da ponte H                            |
| 19   | Motor 1, entrada B da ponte H                            |
| 22   | Motor 2, entrada A da ponte H (lado direito no diferencial) |
| 23   | Motor 2, entrada B da ponte H (lado direito no diferencial) |
| 26   | Sinal PWM do servo de direção (Ackermann)                |
| 25   | Farol PWM                                                |
| 14   | Lanterna                                                 |
| 13   | Luz de freio                                             |
| 12   | Luz de ré                                                |
| 33   | Seta esquerda                                            |
| 32   | Seta direita                                             |
| 5    | Buzina                                                   |

No modo Ackermann, conecte o sinal do servo ao GPIO 26. Alimente o servo por uma fonte dimensionada para o modelo utilizado, não pelo pino 3,3 V do ESP32. Una o GND da fonte ao GND do ESP32 e confirme que o servo aceita sinal lógico de 3,3 V. Os pulsos são configurados inicialmente entre 1000 e 2000 us; ajuste esses limites conforme o servo e a mecânica da direção, evitando forçar o fim de curso.



# Dependências do Projeto

O projeto utiliza bibliotecas externas que devem estar presentes na pasta `components/`.

Atualmente são utilizadas as seguintes versões:

| Componente | Versão | Tag |
|------------|---------|---------|
| Bluepad32  | v4.2.0  | 4.2.0   |
| BTstack    | v1.6.2  | v1.6.2  |

Estrutura esperada:

```text
components/
├── bluepad32/
└── bluepad32/external/btstack/
```

Esses componentes fazem parte da compilação do projeto e devem estar presentes para que o firmware seja compilado corretamente.

---

## Obtendo as dependências

Caso o projeto esteja utilizando **Git Submodules**:

```bash
git clone --recursive <repositorio>

# ou, caso o projeto já tenha sido clonado

git submodule update --init --recursive
```

Caso as dependências não sejam obtidas automaticamente, faça a clonagem manual:

### Bluepad32

```bash
cd components/bluepad32

git clone --branch 4.2.0 https://github.com/ricardoquesada/bluepad32.git
```

### BTstack

```bash
cd components/bluepad32/external

git clone --branch v1.6.2 https://github.com/bluekitchen/btstack.git
```

> **Observação**
>
> O projeto foi desenvolvido e testado utilizando exatamente essas versões. A utilização de versões diferentes pode exigir adaptações no código devido a mudanças na API das bibliotecas.

---

# Ambiente de Desenvolvimento

Ferramentas utilizadas durante o desenvolvimento:

| Ferramenta | Versão         |
|------------|----------------|
| ESP-IDF    | v5.3.5         |
| ESP32      | ESP32-WROOM    |
| Bluepad32  | v4.2.0         |
| BTstack    | v1.6.2         |
| Ninja      | 1.12.1         |
| GCC Xtensa | 13.2.0         |

---

# Compilação

```bash
idf.py build
```

---

# Gravação

```bash
idf.py -p COM4 flash
```

Substitua `COM4` pela porta correspondente ao seu ESP32.

---

# Monitor Serial

```bash
idf.py monitor
```

ou

```bash
idf.py flash monitor
```

---

# Funcionalidades

- [X] Bluetooth via Bluepad32
- [X] Conexão com Gamepad
- [X] Controle diferencial dos motores
- [X] Failsafe
- [X] Arquitetura modular
- [X] Mapeamento de comandos do controle
- [X] Estado centralizado do veículo
- [X] Pisca esquerdo, direito e pisca-alerta
- [X] Farol baixo, médio e alto
- [X] Luz de freio e de ré
- [X] Buzina
- [X] Servo de direção
- [X] Controle de velocidade
- [X] Buzina
- [ ] OTA
- [ ] Interface Web
- [ ] Telemetria
- [ ] Monitoramento de bateria

---

## Filosofia do projeto

Todo o projeto foi desenvolvido para manter desacopladas as seguintes responsabilidades:

- Entrada (Gamepad)
- Lógica do veículo
- Hardware

Isso permite substituir qualquer uma dessas camadas sem impactar as demais.

Exemplos:

- substituir Bluetooth por Wi-Fi;
- utilizar um rádio 2.4 GHz;
- implementar piloto automático;
- adicionar sensores;
- reutilizar os drivers em outro veículo.

---

# Licença

Este projeto é distribuído apenas para fins de estudo e desenvolvimento pessoal.
