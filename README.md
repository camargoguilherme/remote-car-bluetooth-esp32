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
Bluetooth (Bluepad32)
            │
            ▼
     bluetooth/
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
            │
 ┌──────────┼──────────┐
 ▼          ▼          ▼
Motors    Lights      Horn
```

O projeto utiliza uma arquitetura baseada em estados.

- O controle não acessa diretamente o hardware.
- O hardware não conhece o controle Bluetooth.
- Toda comunicação passa por `VehicleCommand` e `VehicleState`.

---


# Dependências do Projeto

O projeto utiliza bibliotecas externas que devem estar presentes na pasta `components/`.

Atualmente são utilizadas as seguintes versões:

| Componente | Versão | Tag |
|------------|---------|------|
| Bluepad32 | v4.2.0 | 4.2.0 |
| BTstack | v1.6.2 | v1.6.2 |

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

| Ferramenta | Versão |
|------------|---------|
| ESP-IDF | v5.3.5 |
| ESP32 | ESP32-WROOM |
| Bluepad32 | v4.2.0 |
| BTstack | v1.6.2 |
| Ninja | 1.12.1 |
| GCC Xtensa | 13.2.0 |

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

# Funcionalidades implementadas

- Bluetooth via Bluepad32
- Conexão com Gamepad
- Controle diferencial dos motores
- Failsafe
- Arquitetura modular
- Mapeamento de comandos do controle
- Estado centralizado do veículo

---

# Funcionalidades planejadas

- [ ] Pisca esquerdo
- [ ] Pisca direito
- [ ] Pisca-alerta
- [ ] Farol baixo
- [ ] Farol médio
- [ ] Farol alto
- [ ] Luz de freio
- [ ] Luz de ré
- [ ] Buzina
- [ ] Servo de direção
- [ ] Controle de velocidade
- [ ] OTA
- [ ] Interface Web
- [ ] Telemetria
- [ ] Monitoramento de bateria

---

# Licença

Este projeto é distribuído apenas para fins de estudo e desenvolvimento pessoal.
