



# Einführung
Bei der Arbeit mit Bildschirmen kann es vorkommen, dass man eine höhere Farbtiefe darstellen will, als es mit der zur Verfügung stehenden Hardware möglich ist. Sei es, dass der Bildschirm oder das verwendete Übertragungsmedium für diese Limitierung sorgt. Trotzdem kann es sein, dass noch Ressourcen in Form von der Aktualisierungsrate oder der Pixeldichte zur Verfügung stehen. Diese Ressourcen können so ausgenutzt werden, dass das Auge mehr Farben und somit eine höhere Farbtiefe wahrnimmt, als die Hardware physikalisch darstellen kann.
#Ziel
Auf den meisten Bildschirmen, die heutzutage Verwendung finden, kann eine Bittiefe von 8 Bit dargestellt werden, was bei den meisten Anwendungsfelder auch keine visuellen Einbußen mit sich bringt. Trotzdem kann es mit den 8 Bit bei Farbverläufen zur Stufenbildung kommen, die unschön in Erscheinung tritt. Die Lösung hierfür ist die Erhöhung der Bittiefe. Will man 12 Bit auf auf einem 8 Bit Bildschirm darstellen, so muss man eine Reduktion um 4 Bit vornhemen während der visuelle Eindruck gleich bleibt. Dies kann dadurch erreicht werden, indem 2 Bit mittels räumliches Dithering und 2 Bit mittels zeitliches Dithering reduziert werden. Mit diesen Gedanken wurde ein VHDL-Block erstellt, der die Farbtiefe um 4 Bit reduzieren kann, wobei der visuelle Eindruck beibehalten wird. Dieser VHDL-Block reduziert in der verwendeten Umgebung ein 8 Bit Signal zu einem 4 Bit Signal.
# Räumliches Dithering
Das menschliche auge hat im besten Fall ein begrenztes räumliches auflösungsvermögen von einer Bogenminute(60 Bogenminuten entspricht einem Grad). Alles was sich horizontal und vertigkal innerhalb dieser Bogenminute befindet sehen wir als einen Punkt, wobei wir die Farbinformationen gemittelt sehen.  Diese Eigenschaft kann bei den meisten Bildschirmen erfüllt werden, wenn man 4 Pixel nimmt, die beieinander liegen. 

Ein Pixel besteht aus einer rot, grün und blau leuchtenden Flächen. Diese entsprechen den Farbkanälen und die Leuchtintensität der Farbkanäle wird mittels binärer Zahlen dargestellt. Will man jetzt eine Reduktion um 2 Bit vornhemen, so löscht man die letzten beiden Bits. Man verliert dabei 4 Farbstufen in dem Farbkanal. Will man die 4 Farbstufen wieder gewinnen, nimmt man 4 benachbarte Pixel zusammen und addiert jeweils auf den gelöschten Farbkanal folgende Muster:
0 0 | 1 0 | 0 1 | 0 1 
0 0 | 0 0 | 1 0 | 1 1
Jede Zahl enspricht einem Pixel, also 2x2 Pixel. Beim ganz linkten Muster hatten die gelöschten Bits den Wert 0 und steigen dann auf bis ganz rechts dem Wert von 3. 

Dies ist prinzipiell die funktionsweise des räumlichen Dithering. In der Implementierung sind diese Muster immernoch vorhanden, allerdings je nach Position rotiert womit in Kombination mit dem zeitlichen Dithering eine bessere Verteilung der hellen und dunklen Farbkanäle der einzelnen Pixel erzeugt wird. 
#Zeitliches Dithering
Beim zeitlichen Dithering wird das begrenzte zeitliche Auflösungsvermögen des Auges ausgenutzt. Die Grenze hierbei liegt bei einer Aktualisierungsrate von 50-60Hz. Wenn also eine Fläche innerhalb des zeitlichen Auflösungsvermögen die Farbe wechselt, nehmen wir diese Farben gemittelt wahr. Beim zeitlichen Dithering löscht man auch die letzten beiden Bits und addiert anschließend zum erhalt der ursprünglichen 4 Farbstufen des Farbkanals folgende Schemen je nach Wert der gelöschten Pixel:

gelöschte Bits haben den Wert Null:  0 | 0 | 0 | 0
gelöschte Bits haben den Wert Eins:  1 | 0 | 0 | 0
gelöschte Bits haben den Wert Zwei:  1 | 0 | 1 | 0
gelöschte Bits haben den Wert drei: 0 | 1 | 1 | 1

In horizontaler Richtung haben wir dieses mal die Framenummer. Die 1 wird in diesem Fall auf den Farbkanal aller Pixel addiert. 

Dies ist prinzipiell die funktionsweise des zeitlichen Ditherings, da wir bei den meisten Bildschirmen schon eine Aktualisierungsrate von 60Hz haben, erhalten wir mit den obigen Schemen Frequenzen, die niedriger liegen. Diese machen sich als sichtbares Flackern bemerkbar. Um dieses Flackern zu minimieren wird der Startpunkt der Schemen räumlich auf 4 benachbarte Pixel verteilt. 
# Verwendung
## Benötigte Hardware
Zybo Z7-20
2 HDMI Kabel
1 Bildschirm, Farbtiefe 8 Bit, Aktualisierungsrate 60Hz
1 Quelle, Farbtiefe 8 Bit im RBG-Format
## Benötigte Software
Matlab R2020a 
 * Vision HDL Toolbox 
 
Xilinx Vivado 2020.1

## Arbeit mit Simulink
1. Zur korrekten Installation der Software: PIXELdithering\Anleitungen\Installationsanleitung.pdf

2. Arbeit mit der Workbench: PIXELdithering/Anleitungen/Laboranleitung.pdf

3. Arbeit mit Dithering: PIXELdithering\Simulink\PIXELdithering_workbench.slx
