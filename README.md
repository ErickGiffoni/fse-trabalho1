# Projeto 1 - 2020/2

Projeto 1 da disciplina de Fundamentos de Sistemas Embarcados (2021/2)

## 1. Objetivos

Este trabalho tem por objetivo a implementação de um sistema (que simula) o controle de um forno para soldagem de placas de circuito impresso (PCBs). 

![Forne de Exemplo - LPKF](https://www.lpkf.com/fileadmin/mediafiles/_processed_/1/b/csm_lpkf_protoflow_s4_05300933fa.png)

o software de uma bancada para testes de controle de temperatura de um sistema que possui internamente dois dispositivos para alterar sua temperatura. O primeiro é um resistor de potência de 15 Watts utilizado para aumentar temperatura e o segundo, uma ventoinha que puxa o ar externo (temperatura ambiente) para reduzir a temperatura do sistema. 

Como requisitos, o usuário do sistema pode definir a temperatura de referência (TR) ou temperatura desejada de duas maneiras:
1. Através de um Potenciômetro;
2. Através de entrada de teclado no terminal.

O software da bancada de testes deve implementar duas estratégias de controle simples: On/Off e PID. O uso de cada estratégia também é definida pelo usuário do sistema através de 2 mecanismos.
1. Através de botão *switch* físico;
2. Através de entrada de teclado no terminal.

## 2. Controle On/Off

A primeira estratégia de controle de temperatura do sistema deve ser realizado utilizando a abordagem de controle On-Off ou Liga-Desliga com histerese. A **histerese** é definida como a diferença entre o limite superior e o inferior da variável a ser controlada. Por exemplo, se a temperatura de operação do sistema for definida como 40˚C com histerese de 4˚C, os limites inferior e superior serão respectivamente de 38˚C e 42˚C.

Neste caso, quando o valor medido de temperatura ficar abaixo do limite inferior, o controlador deverá acionar o resistor de potência para aquecer o sistema. Caso a temperatura medida esteja acima do limite superior, o controlador deve desligar a resistência e acionar a ventoinha.

Essa é a abordagem mais simples de controle em que a fonte de aquecimento ou de resfriamento são ligadas ou desligadas de modo completo. Esta estratégia funciona, porém sua operação impede que o sistema repouse exatamente em uma temperatura desejada, ao invés disso, o sistema oscila em torno da temperatura de referência conforme mostrado na figura abaixo.

<!-- <div style="text-align:center"><img src="/figuras/controle_on_off.png" /></div> -->

![PID](/figuras/controle_on_off.png)

## 3. Controle PID

Na segunda abordagem, deve ser utilizado o controle PID (Proporcional Integral Derivativo). Após o controle On-Off, o PID é um dos tipos mais simples de algoritmos de controle que proporciona um bom desempenho para uma grande variedade de aplicações.

O conceito fundamental desse tipo de controlador se baseia em monitorar uma ou mais variáveis de um processo (neste caso a temperatura interna) e medir a diferença entre seu valor atual (TI) a uma valor de referência (TR) desejado. A partir dessa medida de **Erro = TR - TI**, toma-se uma ação de correção para que o sistema alcançe exatamente o valor desejado.

![Sistema de Controle](https://upload.wikimedia.org/wikipedia/commons/2/24/Feedback_loop_with_descriptions.svg)

O controle PID une três tipos de ações sobre a variável de controle para minimizar o erro do sistema até que o mesmo alcançe a referência desejada. No caso deste sistema, nossa variável de controle é o acionamento da **Resistência (R)** ou da **Ventoinha (V)** e nosso **Erro** é a diferença entre a temperatura de referência e a temperatura interna do sistema (Erro = TR - TI).

Detalhando as 3 ações do PID temos:
- **Controle Proporcional (P)**: ajusta a variável de controle de forma proporcional ao erro, ou seja, quanto maior o erro, maior a intensidade de acionamento do resistor (0 a 100%). Esse ajuste é feito pela variável *Kp*.
- **Controle Integral (PI)**: ajusta a variável de controle baseando-se no tempo em que o erro acontece, acumulando este erro (integral). Esse ajuste é feito pela variável *Ki*.
- **Controle Derivativo (PD)**: ajusta a variável de controle tendo como base a taxa de variação do erro ou a velocidade com a qual o sistema está variando o erro. Esse ajuste é feito pela variável *Kd*.

A figura abaixo mostra as equações envolvidas no cálculo do PID.

![PID](https://upload.wikimedia.org/wikipedia/commons/4/43/PID_en.svg)

O ajustes de cada constantes do PID (Kp, Ki e Kd) tem efeitos distintos no controle do sistema conforme pode ser visto na figura  abaixo.

![PID - Variáveis Kp, Ki, Kd](https://upload.wikimedia.org/wikipedia/commons/3/33/PID_Compensation_Animated.gif)


## 4. Componentes do Sistema

O sistema como um todo é composto por:
1. Ambiente fechado controlado com o resistor de potência e ventoinha;
2. 01 Sensor DS18B20 (1-Wire) para a medição da temperatura interna (TI) do sistema;
3. 01 Sensor BME280 (I2C) para a medição da temperatura externa (TE);
4. 01 módulo Display LCD 16x2 com circuito I2C integrado (Controlador HD44780);
5. 01 Conversor lógico bidirecional (3.3V / 5V);
6. 01 Driver de potência (Ponte H) para acionamento de duas cargas;
6. 01 ESP32;
7. 01 Potenciômetro (Para definir a temperatura de referência TR);
8. 01 Switch (Para definir a estratégia de controle);
9. 01 Raspberry Pi 4;

![Figura](/figuras/Figura_Trabalho_1_v2.png)

## 5. Conexões entre os módulos do sistema

1. O sensor de temperatura BM280 está ligado ao barramento I2C e utiliza o endereço (0x76);
2. O módulo de display LCD está conectado ao barramento I2C utilizando o endereço 0x27;
3. O resistor de potência e a ventoinha estão ambos ligados às portas GPIO e são acionados através do circuito de potência (Ponte H);  
    3.1. Resistor: GPIO 23 ou Pino 16 (Board)  
    3.2. Ventoinha: GPIO 24 ou Pino 18 (Board)  
4. A ESP32 está conectada à placa Raspberry Pi via UART (Protocolo MODBUS-RTU);
5. O potenciômetro é conectado à porta analógica (A1) da ESP32;
6. A chave (switch) está ligada à GPIO 5 da ESP32;
7. O sensor de temperatura DS18B20 para medição do ambiente controlado está ligado à ESP32 na porta GPIO 4 via protocolo 1-Wire;

## 6. Requisitos

Os sistema de controle possui os seguintes requisitos:
1. O código deve ser desenvolvido em C/C++;
2. Na implementação do software, não podem haver loops infinitos que ocupem 100% da CPU;
3. O sistema deve implementar o controle de temperatura do ambiente fechado utilizando as técnicas de controle On-Off e PID atuando sobre o Resistor e a Ventoinha;
4. O sistema deve apresentar na tela LCD os valores das temperaturas (TI, TE, TR)atualizados a cada 1 segundo;
5. O sistema deve apresentar uma interface de controle via terminal com menu de interação com o usuário mostrando atualizados os dados de temperaturas (TI, TE, TR) a cada 1 segundo;
6. A interface de terminal do usuário deve prover a capacidade de definição dos seguintes parâmetros:
   1. **Estratégia de controle (On-Off ou PID)** - deve haver uma opção em que a definição deste parâmetro seja feita pelo teclado ou pela chave externa;
   2. **Temperatura de referência (TR)**: deve haver uma opção para escolher se o sistema irá considerar TR definido pelo teclado ou através do potenciômetro;
   3. **Histerese**: para o controle On-Off seve ser possível definir o valor de histerese.
   4. **Parâmetros Kp, Ki, Kd**: para o controle PID deve ser possível definir os valores das constantes Kp, Ki e Kd;
7. No caso da temperatura ser definida pelo potenciômetro, o programa deve consultar o valor do potenciômetro através da comunicação UART com a ESP32 a cada 1 segundo;
8. O programa deve gerar um log em arquivo CSV das seguintes informações a cada 02 segundos com os seguintes valores: (Data e hora, temperatura interna, temperatura externa, temperatura definida pelo usuário, valor de acionamento dos atuadores (Resistor e Venoinha em valor percentual)).
9.  O programa deve tratar a interrupção do teclado (Ctrl + C = sinal **SIGINT**) encerrando todas as comunicações com periféricos (UART / I2C / GPIO) e desligar os atuadores (Resistor e Ventoinha);
10. O código deve possuir Makefile para compilação;
11. O sistema deve conter em seu README as instruções de compilação e uso, bem como gráficos* com o resultado de dois experimentos, um com o controle On-Off e outro com o PID sendo executado pelo período de 10 minutos com variação da temperatura de referência sendo dada pelo potenciômetro.
 
\* Serão necessários dois gráficos para cada experimento. Um deles plotando as temperaturas (Ambiente, Interna e Referência (Potenciômetro)) e outro gráfico com o valor do acionamento dos atuadores (Resistor / Ventoinha) em valor percentual entre -100% e 100%.

## 7. Comunicação UART com a ESP32

A comunicação com a ESP32 deve seguir o mesmo protocolo MODBUS utilizado no [Exercício 2](https://gitlab.com/fse_fga/exercicios/exercicio-2-uart-modbus).
 
A ESP32 fornece três informações para o sistema, valor da temperatura interna (Sensor DS18B20), valor da temperatura de referência (Potenciômetro) e o valor da chave seletora da estratégia de controle.

Além disso, o valor de acionamento do Resistor / Ventoinha devem ser eviados à ESP32 para um Log interno. Neste caso um valor entre -100 e 100 deve ser enviado em formato inteiro onde: -100 representa a ventoinha ligada em 100% de sua capacidade e 100 representa o resistor ligado a 100% da potência.

Para acessar as informações via UART envie mensagens em formato MODBUS com o seguinte conteúdo:

1. Código do Dispositivo (ESP32): 0x01
2. Leitura de Valor de Temperatura Interna (TI): Código 0x23, Sub-código: 0xC1 + 4 últimos dígitos da matrícula. O retorno será o valor em Float (4 bytes) da temperatura interna do sistema com o pacote no formato MODBUS;
4. Leitura da temperatura de referência - TR (Potenciômetro): Código 0x23, Sub-código: 0xC2 + 4 últimos dígitos da matrícula. O retorno será o valor em Float (4 bytes) da temperatura de referência definida pelo usuário com o pacote no formato MODBUS;
5. Leitura da chave seletora da estratégia de controle: Código 0x23, Sub-código: 0xC3 + 4 últimos dígitos da matrícula. O retorno será o valor em Int (4 bytes) onde 0 = Controle On-Off e 1 = Controle PID;
6. Envio do sinal de controle (Resistor / Ventoinha): Código 0x16,  Sub-código: 0xD1 + 4 últimos dígitos da matrícula, Valor em Int (4 bytes).

<p style="text-align: center;">Tabela 1 - Códigos do Protocolo de Comunicação</p>

| Código |	Sub-código + Matricula | Comando de Solicitação de Dados |	Mensagem de Retorno |
|:-:|:-:|:--|:--|
| **0x23** | **0xC1** N N N N |	Solicita Temperatura Interna  | float (4 bytes) |
| **0x23** | **0xC2** N N N N |	Solicita Temperatura Potenciômetro	| float (4 bytes) |
| **0x23** | **0xC3** N N N N |	Solicita Estado da Chave (On-Off / PID) | int (4 bytes) |
| **0x16** | **0xD1** N N N N |	Envia sinal de controle Int (4 bytes) | - |

## 8. Parâmetros de PID

Para o uso do controle do PID, estão sendo sugerido os seguintes valores para as constantes:

- **Kp** = 5.0
- **Ki** = 1.0
- **Kd** = 5.0

Porém, vocês estão livres para testar outros valores que sejam mais adequados.

### Acionamento do Resistor 

O **resistor** deve ser acionado utilizando a técnica de PWM (sugestão de uso da biblioteca WiringPi / SoftPWM). A intensidade de acionamento do resistor por variar entre 0 e 100%.

### Acionamento da Ventoinha

A **venotinha** também deve ser acionada utilizando a técnica de PWM. Porém, há um limite inferior de 40% de intensidade para seu acionamento pelas características do motor elétrico. Ou seja, caso o valor de saída do PID esteja entre 0 e -40%, tanto a ventoinha quanto o resistor devem permanecer desligados sendo que a ventoinha só deve ser acionada quando a saída PID estiver abaixo de -40%.

Observa-se ainda que a saída negativa só indica que o atuador a ser acionado deve ser a ventoinha e não o resistor e o valor de PWM a ser definido deve ser positivo, invertendo o sinal.

## 9. Critérios de Avaliação

A avaliação será realizada seguindo os seguintes critérios:

|   ITEM    |   COMENTÁRIO  |   VALOR   |
|------------------------|---------------------------------------------------------------------------------------------------------|---------|
|**Implementação do controlador On-Off** | Correta implementação do controlador PID (Resistor / Venotinha), incluindo a leitura das temperaturas e acionamento dos atuadores. |    1,0 |
|**Implementação do controlador PID** | Correta implementação do controlador PID (Resistor / Venotinha), incluindo a leitura das temperaturas e acionamento dos atuadores. |    1,5 |
|**Menu de controle**        | Correta implementação do menu apresentando as informações atualizadas ao usuário e permitindo o controle dos parâmetros do sistema. | 1,5 |
|**Leitura da Temperatura Ambiente**| Leitura dos valores de Temperatura Ambiente (Sensor BME280). | 0,5 |
|**Comunicação UART** | Leitura dos valores de Temperatura Interna, Potenciômetro e Chave e envio do sinal de controle através da comunicação MODBUS-UART. | 1,0 |
|**Mostrador no LCD**        | Apresentação das 3 temperatudas no LCD. | 1,0 |
|**Armazenamento em arquivo**| Armazenamento em arquivo CSV dos dados medidos. |   0,5 |
|**Qualidade do Código**     | Utilização de boas práticas como o uso de bons nomes, modularização e organização em geral.    |  2,0 |
|**README com Experimento** | Documentação README com instruçoes de compilaçõa, uso e relatório do experimento com o gráfico. |  1,0 |
|**Pontuação Extra**         |   Qualidade e usabilidade acima da média.  |  0,5   |

## 10. Referências

[Controle Liga/Desliga - Wikipedia](https://pt.wikipedia.org/wiki/Controle_liga-desliga)  
[Controle PID - Wikipedia](https://pt.wikipedia.org/wiki/Controlador_proporcional_integral_derivativo)  
[Driver da Bosh para o sensor BME280](https://github.com/BoschSensortec/BME280_driver)  
[Biblioteca BCM2835 - GPIO](http://www.airspayce.com/mikem/bcm2835/)  
[Controle do LCD 16x2 em C](http://www.bristolwatch.com/rpi/i2clcd.htm)  
[Biblioteca WiringPi GPIO](http://wiringpi.com)  
[PWM via WiringPi](https://www.electronicwings.com/raspberry-pi/raspberry-pi-pwm-generation-using-python-and-c)



