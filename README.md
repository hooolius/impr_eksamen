# Løsningsforslag til eksamensopgave i imperitiv programmering, 2018 AAU

## Opgavebeskrivelse
Hvert forår afvikles der et antal éndags-cykelløb - såkaldte forårsklassikere. I denne opgave gives der data fra fire sådanne cykelløb fra foråret 2017, nemlig Paris Roubaix, Amstel Gold Race, La Fleche Wallonne og Liege Bastogne Liege. Alle data stammer fra UCI (den internationale cykelunion) http://www.uci.ch/road/results/. De hentede data fra UCI er bearbejdet på forskellig vis, så de er egnede som udgangspunkt for denne opgave.

Du skal først indlæse alle løbsdata fra den givne tekstfil til et array af cykelløbsresultater (et array af structs). I dit program skal tekstfilen have navnet "cykelloeb". Hver linje i datafilen repræsenterer ét resultat fra ét cykelløb for én rytter. Formatet af en linje i tekstfilen er:
```
   løbsnavn rytternavn | rytter-alder rytter-hold nationalitet | placering køretid
```
Løbsnavnet angives på sammentrukket vis, i ét ord uden mellemrum. Rytterens navn angives i dobbelt quotes "...", med efternavnet angivet ved brug af store bogstaver. Rytterens alder angives som et heltal. Holdet, som rytteren er tilknyttet, er angivet med en forkortelse på tre store bogstaver. Ligeledes er rytterens nationalitet angivet som en forkortelse på tre store bogstaver. Placeringen er et heltal, hvor tallet 1 betegner vinderen af løbet. Placeringen kan også angives som DNF (did not finish) som tegn på at rytteren ikke gennemførte løbet, eller som OTL (over time limit) som tegn på at rytteren ikke kom i mål inden for løbets tidsgrænse (og dermed ikke opnår en egentlig placering). Køretiden er angivet i timer, minutter og sekunder adskilt med tegnet kolon. Ryttere som ikke har gennemført løbet har en køretid, der er angivet med tegnet '-'. De syv bestanddele af et løbsresultat er adskilt med et eller flere mellemrum (spaces), dog således at gruppen af alder, hold og nationalitet er omkranset med tegnet '|'. Her er et eksempel på et løbsresultat:
```
   ParisRoubaix               "Greg VAN AVERMAET"                          | 32  BMC   BEL |      1      5:41:07
```
Det betyder at den 32 årige belgiske cykelrytter Greg Van Avermaet, som kører på holdet BMC, blev nummer ét i Paris Roubaix i tiden 5 timer, 41 minutter og 7 sekunder.

Cykelrytterne tildeles et antal point (et positivt heltal) ud fra deres deltagelse, og deres opnåede resultater, i de cykelløb som indgår i datamaterialet. Her er reglerne for tildeling af points:

* Deltagelsespoint: Deltagelse og gennemførelse i et cykelløb inden for tidsgrænsen giver 3 point. Deltagelse og gennemførelse i et cykelløb uden for tidsgrænsen giver 1 point.
* Placeringspoint: Hvis man gennemfører cykelløbet uden at overskride løbets tidsgrænse får man (M - P)/13 ekstra-point, hvor M er antallet af ryttere der har kørt løbet (incl. de ryttere der har fået DNF) og P er rytterens placering i løbet.
* Podiepoint: I tillæg til dette får vinderen af et cykelløb 10 ekstra point, nummer to får 5 ekstra point, og nummer tre får 2 ekstra point.

På baggrund af de indlæste data om løbsresultaterne skal du skrive et C program med følgende problemløsningsfunktioner:

1. Find og udskriv løbsresultaterne for alle italienske cykelryttere over 30 år. I denne opgave er det OK at lave en funktion som blot printer resultaterne direkte.
2. Skriv en funktion som returnerer et array af alle de danske ryttere, som har gennemført et eller flere af de fire cykelløb, enten med en placering eller med OTL. Kald funktionen, og udskriv disse danske ryttere samt hvor mange løb hver af de danske ryttere har gennemført. Bemærk, at hver rytter kun ønskes udskrevet én gang.
3. Udskriv de 10 ryttere som har opnået flest point. Sorter primært rytterne efter antal point. Ved pointlighed sorteres rytterne alfabetisk efter efternavn. (Efternavnet er den del af rytterens navn som er skrevet med udelukkende store bogstaver. Bemærk at efternavnet også kan indeholde mellemrum og specialtegn).
4. I denne opgave fokuseres på de to cykelløb Paris Roubaix og Amstel Gold Race. Skriv en funktion som finder den rytter, som har gennemført begge disse løb, og som har den mindste samlede køretid i de to løb. Funktionen skal have rytteren og den samlede køretid som outputparametre. Kald funktionen og udskriv rytterens navn og den samlede køretid (i timer, minutter og sekunder).
5. Beregn gennemsnitsalderen (af typen double) blandt alle de ryttere, som har opnået en topti placering i et eller flere cykelløb. Vær sikker på at ryttere, som har et top-ti resultat i to eller flere af de fire løb, ikke indgår mere én gang i gennemsnitsberegningen.

Skriv en main funktion, som i en simpel tekst-orienteret dialog med brugeren tillader aktivering af ovennævnte funktioner. Det anbefales at denne del af programmet laves så simpel som mulig.

Som altid anbefales det at programmere én eller flere funktioner pr. opgave, herunder en funktion, der beregner opgavens resultat (gennem returværdi eller parameter).

Lad os antage at dit oversatte program er på filen cykling.exe. Når dit program køres som
```
  cykling.exe --print
```
skal det - uden at modtage nogen form for input fra brugeren - udskrive dine resultater for alle opgaver på en klar og tydelig måde på standard output. (Brug programparametre til main for at opnå dette).

Når programmet startes uden angivelse af --print skal det udføre den (simple) brugerdialog.

I denne opgave er det OK at anvende danske navne på variable og funktioner. Undgå dog, som sædvanlig i C programmering, brugen af danske bogstaver (æ, ø, å, Æ, Ø og Å) i variabelnavne og funktionsnavne.

Når dit program skal vurderes vil der blive lagt vægt på følgende egenskaber:

* Læsbarhed og forståelighed, herunder god og klar programopstilling (indrykning mv.) samt fornuftig anvendelse af programkommentarer.
* God navngivning af variable, parametre og funktioner, der afspejler den rolle som navnene spiller i programmet.
* Fornuftig nedbrydning af problemløsningen i et passende antal funktioner, som følge af top-down programmering ved trinvis forfinelse (del og hersk).
* God parametrisering af funktionerne, således at alt input til en funktion foregår gennem input parametre, og at output fra en funktionen formidles gennem returværdien eller output parametre.
* Fravær af globale variable.
* Valg af hensigtsmæssige datastrukturer (structs og arrays).
* Hensigtsmæssig anvendelse af funktioner fra C Standard Libraries.

Som følge af den begrænsede mængde af data, som skal behandles i denne opgave, prioriteres et enkelt og klart kildeprogram højere end effektivitet af beregningerne.

Evt. uklarheder i opgaveformuleringen forventes afklaret som en del af opgaveløsningen (gennem kommentarer i det afleverede program).
