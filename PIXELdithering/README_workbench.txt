# P!X3LFLUX FPGA Laborversuche
Willkommen zu den FPGA Laborversuchen zum Labor P!X3LFLUX der Hochschule Karlsruhe - Technik und Wirtschaft.

In diesem Versuch werden Sie behutsam in die wunderbare Welt der Echtzeitvideoverarbeitung mit FPGAs eingeführt. Und das ohne dabei eine einzige Zeile VHDL Code zu schreiben!

## Getting started!

1. Laden Sie dieses Repository herunter.
2. Falls Sie auf Ihrem privaten Rechner arbeiten möchten, folgen Sie der Installationsanleitung.
3. Sehen Sie sich die Einführungsvideos vor dem Labortermin an.
4. Arbeiten Sie das Tutorialvideo am Versuchstag durch.
5. Have fun!

## Was ist drin?

- **Anleitungen**: Enthält die Laboranleitung und eine Installationsanleitung für die Einrichtung der benötigten Software.
- **Simulink**: Enhält die P!X3LFLUX Simulink Workbenches für die Teilversuche.
- **Testdaten**: Enthält einige freie Videos für die Durchführung der Simulationen.
- **Videos**: Enthält die Einführungsvideos und das Tutorial.
- **zyboZ7_board_definitions** Enthält die MATLAB HDL Coder Definitionen für das Digilent Zybo Z7 sowie das FPGA-Referenzdesign.

## Requirements
Für die Laborversuche wird die nachfolgend aufgelistete Software benötigt.
Auf den Rechnern im Labor steht bereits alles notwendige zur Verfügung.
Die Installationsanleitung führt Sie durch die Einrichtung.

- **Xilinx Vivado 2020.1** (WebPACK Editition, Zynq7000 Series Support)
- **Matlab 2020a** mit folgenden Toolboxen/Add-Ons:
    - Simulink
    - HDL Coder
    - HDL Verifier
    - Fixed Point Designer
    - Vision HDL Toolbox
    - Computer Vision Toolbox
    - Image Processing Toolbox
    - HDL Coder Support Package for Xilinx Zynq Platform
    - Computer Vision Toolbox Support Package for Xilinx Zynq-Based Hardware

## Third-Party OpenSource Inhalte
Für die Realisierung des FPGA-Referenzdesigns wurden einige von Digilents OpenSource (MIT Lizenz) IP-Cores verwendet, welche in diesem Repository enthalten sind:

- **dvi2rgb**: Für die Decodierung des eingehenden HDMI Signals
- **rgb2dvi**: Für die Codierung des ausgehenden HDMI Signals
- **tmds_v1_0**: TMDS Definitionen, welche für die Funktion der dvi2rgb und rgb2dvi IP-Cores benötigt werden.

Die IP-Cores sind unter **zyboZ7_board_definitions/+ZyboZ720/+simple_axis_reference/ipcore** abgelegt.