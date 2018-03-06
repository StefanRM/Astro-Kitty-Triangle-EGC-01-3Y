.___________. _______ .___  ___.      ___          __  
|           ||   ____||   \/   |     /   \        /_ | 
`---|  |----`|  |__   |  \  /  |    /  ^  \        | | 
    |  |     |   __|  |  |\/|  |   /  /_\  \       | | 
    |  |     |  |____ |  |  |  |  /  _____  \      | | 
    |__|     |_______||__|  |__| /__/     \__\     |_| 

Nume:    Maftei Stefan - Radu
Grupa:   336CC
Materie: EGC

	Jocul AstroKitty are ca scop controlul unui astronaut printre platforme si
asteroizi pana ajunge la o pozitie de sfarsit (platforma de finish).

1. Controlul Astronautului
	- pentru alegerea directiei astronautului se da click intr-un punct al
ecranului, fiind intregistrat ca eveniment in care se retin coordonatele
punctului unde s-a dat click;
	- astfel folosind punctul curent (de unde pleaca) si punctul destinatie
(unde trebuie sa ajunga) vom calcula panta pentru a putea calcula unghiul cu
care trebuie rotit triunghiul (astronautul) ca sa fie varful pe directia
dorita de utilizator;
	- pentru ca arctangenta (atan) returneaza valori intre -pi/2 si pi/2, in
cazul miscarii descendente adaugam un pi valorii returnate si astfel ne asigu-
ram toate unghiurile posibile;
	- odata dat click, astonautul intra in modul de miscare si nu se va opri
decat la locurile de stationare;
	- miscarea astronautului e caracterizata prin ascendenta/descendenta si
spre dreapta/stanga (valori inregistrate in booleene).

2. Coliziunea cu platformele si meteoritii
	- pentru a detecta o coliziune am incadrat astronautul intr-un cerc, iar
platforma si meteoritii intr-un dreptunghi, si am verificat coliziunea pornind
de la centrele lor de greutate si distanta pana la laturile dreptunghiului.

3. Platforma de Stationare si Peretii
	- peretii sunt interpretati ca platforme de stationare;
	- odata detectata coliziunea, in functie de modul de miscare
ascendenta/descendenta, se va aseza la mijlocul platformei orientat
perpendicular in jos, respectiv in sus.

4. Platforma de Reflexie si Meteoritii
	- meteoritii sunt incadrati in patrate si sunt tratate ca platforme de
reflexie care se translateaza, se rotesc sau se scaleaza;
	- avem 4 cazuri pentru reflexie si anume cei 4 pereti ai platformei,
impreuna cu modul de miscare spre punctul de coliziune intre astronaut si
platforma;
	- dupa detectarea coliziunii, in functie de caz se roteste astronautul
pentru a fi pe directie cu locul unde va fi reflectat;
	- fiind rotit in jurul varfului (din cauza constructiei) rotatia va rezulta
in platforma, deci va fi translatat in functie de varfurile bazei triunghiului
in afara platformei;
	- dupa coliziunea cu meteoritii, acestia din urma sunt eliminati din scena
(sunt distrusi).

5. Platforma de Finish
	- este asemenea platformei de stationare, dar odata atinsa nu se mai
permite niciun fel de eveniment in cadrul jocului.